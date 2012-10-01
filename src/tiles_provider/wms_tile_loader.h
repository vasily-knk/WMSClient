#pragma once

#include "tile_loader.h"
#include "png_cache.h"

class wms_tile_loader : public tile_loader
{
public:
    explicit wms_tile_loader(boost::asio::io_service &io, const string &host);
    void set_png_cache(png_cache *cache);

public:
    shared_ptr<const tile_t> request_tile(const tile_id_t &id);

private:
    class request;
    void tile_ready(const tile_id_t &id);

private:
    const string host_;
    boost::asio::io_service *io_;

    typedef unordered_map<tile_id_t, shared_ptr<request>> requests_map_t;
    requests_map_t requests_;
    png_cache *cache_;
};

