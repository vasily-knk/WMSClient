#include "stdafx.h"
#include "wms_png_provider.h"

using boost::asio::ip::tcp;

class wms_png_provider::request
{
public:
    request(const tile_id_t &id, boost::asio::io_service &io, const string &host, png_callback final_callback);
    ~request();

    void start(shared_ptr<wms_png_provider::request> myself);


private:
    string get_path() const;
    void error(const boost::system::error_code& err);
    void error(const string& message);

    void handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator);
    void handle_connect(const boost::system::error_code& err);
    void handle_write_request(const boost::system::error_code& err);
    void handle_read_status_line(const boost::system::error_code& err);
    void handle_read_headers(const boost::system::error_code& err);
    void handle_read_content(const boost::system::error_code& err);

    void process_response();
    void done();


private:
    const string host_;
    tile_id_t tile_id_;
    boost::asio::io_service *io_;
    shared_ptr<wms_png_provider::request> myself_;


private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
    shared_ptr<png_t> png_;
    png_callback final_callback_;
};


wms_png_provider::request::request(const tile_id_t &id, boost::asio::io_service &io, const string &host, png_callback final_callback)
    : host_(host)
    , tile_id_(id)
    , resolver_(io)
    , socket_(io)
    , png_(new png_t)
    , final_callback_(final_callback)
{
    png_->reserve(50 * 1024);
}

wms_png_provider::request::~request()
{

}


string wms_png_provider::request::get_path() const
{
    std::stringstream ss;
    const double a = 20037508.3427892430765884088807;
    const double epsilon = 1;
    const double stride = a * 2.0 / pow(2.0, tile_id_.zoom);

    auto conv = [&](const int pos) -> double
    {
        return (-a + stride * pos);
    };

    ss << std::fixed;
    ss << conv(tile_id_.x) + epsilon << ",";
    ss << conv(tile_id_.y) + epsilon << ",";
    ss << conv(tile_id_.x + 1) << ",";
    ss << conv(tile_id_.y + 1);

    return ss.str();
}

void wms_png_provider::request::start(shared_ptr<wms_png_provider::request> myself)
{
    myself_ = myself;    

    // Form the request. We specify the "Connection: close" header so that the
    // server will close the socket after transmitting the response. This will
    // allow us to treat all data up until the EOF as the content.
    
    const string coord = get_path();
    //std::cout << "Coord: " << coord << std::endl;
    
    std::ostream request_stream(&request_);
    request_stream << "GET ";
    request_stream << "/cgi-bin/tilecache.cgi?LAYERS=osm&SERVICE=mapnik&VERSION=1.1.1&REQUEST=GetMap&BBOX=";
    request_stream << coord;
    request_stream << " HTTP/1.0\r\n";
    request_stream << "Host: " << host_ << "\r\n";
    request_stream << "Accept: */*\r\n";
    request_stream << "Connection: close\r\n\r\n";

    // Start an asynchronous resolve to translate the server and service names
    // into a list of endpoints.
    auto callback = boost::bind(&wms_png_provider::request::handle_resolve, this, boost::asio::placeholders::error, boost::asio::placeholders::iterator);
    
    tcp::resolver::query query(host_, "http");
    resolver_.async_resolve(query, callback);

}

void wms_png_provider::request::handle_resolve(const boost::system::error_code& err, tcp::resolver::iterator endpoint_iterator)
{
    if (err)
    {
        error(err);
        return;
    }

    auto callback = boost::bind(&wms_png_provider::request::handle_connect, this, boost::asio::placeholders::error);
    boost::asio::async_connect(socket_, endpoint_iterator, callback);
}

void wms_png_provider::request::handle_connect(const boost::system::error_code& err)
{
    if (err)
    {
        error(err);
        return;
    }

    auto callback = boost::bind(&wms_png_provider::request::handle_write_request, this, boost::asio::placeholders::error);
    boost::asio::async_write(socket_, request_, callback);
}

void wms_png_provider::request::handle_write_request(const boost::system::error_code& err)
{
    if (err)
    {
        error(err);
        return;
    }

    auto callback = boost::bind(&wms_png_provider::request::handle_read_status_line, this, boost::asio::placeholders::error);
    boost::asio::async_read_until(socket_, response_, "\r\n", callback);
}

void wms_png_provider::request::handle_read_status_line(const boost::system::error_code& err)
{
    if (err)
    {
        error(err);
        return;
    }

    std::istream response_stream(&response_);
    std::string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    std::string status_message;
    std::getline(response_stream, status_message);

    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
        error("Invalid response");
        return;
    }

    if (status_code != 200)
    {
        std::stringstream ss;
        ss << "Response returned with status code " << status_code;
        error(ss.str());
        return;
    }

    // Read the response headers, which are terminated by a blank line.
    auto callback = boost::bind(&wms_png_provider::request::handle_read_headers, this, boost::asio::placeholders::error);
    boost::asio::async_read_until(socket_, response_, "\r\n\r\n", callback);
}

void wms_png_provider::request::handle_read_headers(const boost::system::error_code& err)
{
    if (err)
    {
        error(err);
        return;
    }

    // Process the response headers.
    std::istream response_stream(&response_);
    std::string header;
    while (std::getline(response_stream, header) && header != "\r")
    {
    }


    // Write whatever content we already have to output.
    if (response_.size() > 0)
        process_response();

    // Start reading remaining data until EOF.
    auto callback = boost::bind(&wms_png_provider::request::handle_read_content, this, boost::asio::placeholders::error);
    boost::asio::async_read(socket_, response_, boost::asio::transfer_at_least(1), callback);

}

void wms_png_provider::request::handle_read_content(const boost::system::error_code& err)
{
    if (err)
    {
        if (err == boost::asio::error::eof)
            done();
        else
            error(err);

        return;
    }

    // Write all of the data that has been read so far.
    process_response();

    // Continue reading remaining data until EOF.

    auto callback = boost::bind(&wms_png_provider::request::handle_read_content, this, boost::asio::placeholders::error);
    boost::asio::async_read(socket_, response_, boost::asio::transfer_at_least(1), callback);
}

void wms_png_provider::request::process_response()
{
    std::istreambuf_iterator<char> src(&response_);
    std::istreambuf_iterator<char> eos;

    std::copy(src, eos, std::back_inserter(*png_));
}

void wms_png_provider::request::error(const boost::system::error_code& err)
{
    error(err.message());
}

void wms_png_provider::request::error(const string& message)
{
    std::cerr << "WMS loader error: " << message << std::endl;
    done();
}

void wms_png_provider::request::done()
{
    final_callback_(png_);
    
    // ready to be deleted
    myself_.reset();
}


















wms_png_provider::wms_png_provider(boost::asio::io_service &io, const string &host)
    : io_(&io)
    , host_(host)
{

}

bool wms_png_provider::request_png(const tile_id_t &id, const png_callback &callback)
{
    shared_ptr<request> r (new request(id, *io_, host_, callback));
    r->start(r);


    //callback(shared_ptr<png_t>(new png_t));
    return true;
}




