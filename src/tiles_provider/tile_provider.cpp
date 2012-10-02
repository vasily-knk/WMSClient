#include "stdafx.h"
#include "tile_provider.h"

size_t tile_t::num_tiles_ = 0;


tile_provider::tile_provider()
    : work_(asio_)
    , runner_thread_(boost::bind(&boost::asio::io_service::run, &asio_))
    , converter_(::get_png_converter())
{
/*
    shared_ptr<dummy_cache> cache(new dummy_cache);
    
    loaders_.push_back(cache);
    loaders_.push_back(shared_ptr<png_provider>(new wms_png_provider(asio_, wms_host)));

    png_cache_ = cache;
*/
}

tile_provider::~tile_provider()
{
    asio_.stop();
    runner_thread_.join();
}

shared_ptr<const tile_t> tile_provider::request_tile(const tile_id_t &id)
{
    shared_ptr<tile_t> tile;
    
    // tiles_in_progress_ critical section
    {
        boost::lock_guard<boost::mutex> lock(tiles_in_progress_mutex_);

        auto it = tiles_in_progress_.find(id);
        if (it != tiles_in_progress_.end())
            return it->second;

        tile.reset(new tile_t);
        tiles_in_progress_[id] = tile;
    }


    BOOST_FOREACH(const provider_cache_pair &p, loaders_)
    {
        auto callback = boost::bind(&tile_provider::png_ready_callback, this, id, tile, _1, p.second);
        
        if (p.first->request_png(id, callback))
            break;
    }

    return tile;
}

void tile_provider::png_ready_callback(const tile_id_t &id, shared_ptr<tile_t> tile, shared_ptr<const png_t> png, shared_ptr<png_cache> cache)
{
    if (cache)
        cache->cache_png(id, png);

    converter_(&(png->at(0)), tile->get_data(), png->size(), tile_t::WIDTH, tile_t::HEIGHT);

    // tiles_in_progress_ critical section
    {
        boost::lock_guard<boost::mutex> lock(tiles_in_progress_mutex_);
        tiles_in_progress_.erase(id);
    }

    tile->set_ready(true);
}

void tile_provider::add_provider(shared_ptr<png_provider> provider, shared_ptr<png_cache> cache)
{
    loaders_.push_back(make_pair(provider, cache));
}

void tile_provider::add_provider(shared_ptr<png_provider> provider)
{
    shared_ptr<png_cache> cache;
    add_provider(provider, cache);
}

