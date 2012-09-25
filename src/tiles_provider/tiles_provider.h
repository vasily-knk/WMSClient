#pragma once

#include "tile_loader.h"
#include "dummy_tile_loader.h"

class tiles_provider : public tile_loader
{
public:
    tiles_provider();
    ~tiles_provider();

    shared_ptr<const tile_t> request_tile(const tile_id_t &id);

private:
    boost::asio::io_service asio_;
    boost::asio::io_service::work work_;
    boost::thread runner_thread_;

    typedef shared_ptr<tile_loader> loader_ptr;
    list<loader_ptr> loaders_;
};
