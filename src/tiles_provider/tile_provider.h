#pragma once

#include "png_provider.h"
#include "png_cache.h"
#include "png_converter.h"

class tile_provider
{
public:
    typedef pair<shared_ptr<png_provider>, shared_ptr<png_cache>> provider_cache_pair;
public:
    tile_provider();
    ~tile_provider();

    shared_ptr<const tile_t> request_tile(const tile_id_t &id);

    void add_provider(shared_ptr<png_provider> provider, shared_ptr<png_cache> cache);
    void add_provider(shared_ptr<png_provider> provider);

    boost::asio::io_service &io_service() 
    {
        return asio_;
    }

private:
    void png_ready_callback(const tile_id_t &id, shared_ptr<tile_t> tile, shared_ptr<const png_t> png, shared_ptr<png_cache> cache);

private:
    boost::asio::io_service asio_;
    boost::asio::io_service::work work_;
    boost::thread runner_thread_;

    list<provider_cache_pair> loaders_;
    shared_ptr<png_cache> png_cache_;
    png_converter converter_;
    
    unordered_map<tile_id_t, shared_ptr<const tile_t>> tiles_in_progress_;
    boost::mutex tiles_in_progress_mutex_;
};

