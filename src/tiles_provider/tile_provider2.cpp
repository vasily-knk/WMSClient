#include "stdafx.h"
#include "tile_provider2.h"
#include "wms_png_provider.h"
#include "png_converter.h"

size_t tile_t::num_tiles_ = 0;


tile_provider2::tile_provider2(const string &wms_host)
    : work_(asio_)
    , runner_thread_(boost::bind(&boost::asio::io_service::run, &asio_))
    , converter_(::get_png_converter())
{
    loaders_.push_back(shared_ptr<png_provider>(new wms_png_provider(asio_, wms_host)));
}

tile_provider2::~tile_provider2()
{
    asio_.stop();
    runner_thread_.join();
}

shared_ptr<const tile_t> tile_provider2::request_tile(const tile_id_t &id)
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

    auto callback = boost::bind(&tile_provider2::png_ready_callback, this, id, tile, _1);

    BOOST_FOREACH(const png_provider_ptr &p, loaders_)
    {
        if (p->request_png(id, callback))
            break;
    }

    return tile;
}

void tile_provider2::png_ready_callback(const tile_id_t &id, shared_ptr<tile_t> tile, shared_ptr<const png_t> png)
{
    converter_(&(png->at(0)), tile->get_data(), png->size(), tile_t::WIDTH, tile_t::HEIGHT);

    // tiles_in_progress_ critical section
    {
        boost::lock_guard<boost::mutex> lock(tiles_in_progress_mutex_);
        tiles_in_progress_.erase(id);
    }

    tile->set_ready(true);
}

