#include "stdafx.h"
#include "wms_png_provider.h"

using boost::asio::ip::tcp;

class wms_tile_loader::request
{
public:
    request(const tile_id_t &id, boost::asio::io_service &io, const string &host);
    ~request();

    shared_ptr<const tile_t> get_tile() const
    {
        return tile_;
    }

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
    void tile_ready();


private:
    const string request_string_;
    boost::asio::io_service *io_;
    shared_ptr<


private:
    tcp::resolver resolver_;
    tcp::socket socket_;
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
};


wms_tile_loader::request::request(const tile_id_t &id, boost::asio::io_service &io, const string &host)
{

}

string wms_tile_loader::request::get_path() const
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


















wms_png_provider::wms_png_provider(boost::asio::io_service &io, const string &host)
    : io_(&io)
    , host_(host)
{

}

bool wms_png_provider::request_png(const tile_id_t &id, const png_callback &callback)
{
    callback(shared_ptr<png_t>(new png_t));
    return true;
}




