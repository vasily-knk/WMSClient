#pragma once

#include "png_provider.h"
#include "png_cache.h"
#include "png_converter.h"

class tile_provider2
{
public:
    tile_provider2(const string &wms_host);
    ~tile_provider2();

    shared_ptr<const tile_t> request_tile(const tile_id_t &id);

private:
    void png_ready_callback(const tile_id_t &id, shared_ptr<tile_t> tile, shared_ptr<const png_t> png);

private:
    boost::asio::io_service asio_;
    boost::asio::io_service::work work_;
    boost::thread runner_thread_;

    typedef shared_ptr<png_provider> png_provider_ptr;
    list<png_provider_ptr> loaders_;
    shared_ptr<png_cache> png_cache_;
    png_converter converter_;
    
    unordered_map<tile_id_t, shared_ptr<const tile_t>> tiles_in_progress_;
    boost::mutex tiles_in_progress_mutex_;
};

