#include "stdafx.h"
#include "tiles_provider.h"

tiles_provider::tiles_provider()
    : work_(asio_)
    , runner_thread_(boost::bind(&boost::asio::io_service::run, &asio_))
{
    loaders_.push_back(loader_ptr(new dummy_tile_loader(asio_)));
}

tiles_provider::~tiles_provider()
{
    asio_.stop();
    runner_thread_.join();
}

shared_ptr<const tile_t> tiles_provider::request_tile(const tile_id_t &id)
{
    for (auto it = loaders_.cbegin(); it != loaders_.cend(); ++it)
    {
        const shared_ptr<const tile_t> tile = (*it)->request_tile(id);
        if (tile)
            return tile;
    }
    return shared_ptr<const tile_t>();
}
