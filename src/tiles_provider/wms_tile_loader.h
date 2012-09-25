#pragma once

#include "tile_loader.h"

class wms_tile_loader : public tile_loader
{
public:
    explicit wms_tile_loader(boost::asio::io_service &io);

public:
    shared_ptr<const tile_t> request_tile(const tile_id_t &id);

private:
    class request;
    void tile_ready(shared_ptr<request>);

private:
    boost::asio::io_service *io_;
};

