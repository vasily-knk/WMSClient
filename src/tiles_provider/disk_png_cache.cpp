#include "stdafx.h"
#include "disk_png_cache.h"

disk_png_cache::disk_png_cache(const string &path, boost::asio::io_service &io)
    : path_(path)
    , io_(&io)
{

}

bool disk_png_cache::request_png(const tile_id_t &id, const png_callback &callback)
{
    // THREAD: client

    const string fullpath = fullpath_for_tile(id);
    
    shared_ptr<std::ifstream> stream(new ifstream(fullpath.c_str(), ifstream::binary | ifstream::in));
    
    if (!stream->is_open())
        return false;

    auto f = boost::bind(&disk_png_cache::load_png, this, stream, callback);
    io_->post(f);

    return true;
}

void disk_png_cache::cache_png(const tile_id_t &id, shared_ptr<const png_t> png)
{
    // THREAD: service

    const string fullpath = fullpath_for_tile(id);
    ofstream s(fullpath.c_str(), ofstream::binary | ofstream::out);
    if (!s.is_open())
        return;

    std::ostreambuf_iterator<char> dst(s);
    std::copy(png->begin(), png->end(), dst);
}

void disk_png_cache::load_png(shared_ptr<std::istream> stream, const png_callback &callback)
{
    // THREAD: service

    shared_ptr<png_t> png(new png_t);

    std::istreambuf_iterator<char> src(*stream);
    std::istreambuf_iterator<char> eos;

    std::copy(src, eos, std::back_inserter(*png));

    callback(png);
}

string disk_png_cache::filename_for_tile(const tile_id_t &id)
{
    std::stringstream ss;
    ss.width(2);
    ss.fill('0');
    ss << id.zoom << "_";
    ss.width(6);
    ss.fill('0');
    ss << id.x << "_";
    ss.width(6);
    ss.fill('0');
    ss << id.y;
    return ss.str();
}

string disk_png_cache::fullpath_for_tile(const tile_id_t &id)
{
    return (path_ + "/" + filename_for_tile(id) + ".png");
}

