#pragma once

#include "png_provider.h"
#include "png_cache.h"

class disk_png_cache : public png_provider_cache
{
public:
    disk_png_cache(const string &path, boost::asio::io_service &io);

    bool request_png(const tile_id_t &id, const png_callback &callback);
    void cache_png(const tile_id_t &id, shared_ptr<const png_t> png);

private:
    static string filename_for_tile(const tile_id_t &id);
    string fullpath_for_tile(const tile_id_t &id);
    void load_png(shared_ptr<std::istream> stream, const png_callback &callback);

private:
    const string path_;
    boost::asio::io_service *io_;
};