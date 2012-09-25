#pragma once

#include "tile_loader.h"

class dummy_tile_loader : public tile_loader
{
public:
    dummy_tile_loader(boost::asio::io_service &io);
    shared_ptr<const tile_t> request_tile(const tile_id_t &id);

private:
    typedef boost::asio::deadline_timer timer_t;

private:
    void callback(const shared_ptr<tile_t> &dst, const shared_ptr<timer_t> &timer);
    void fill_tile(tile_t &tile);

private:
    boost::asio::io_service *io_;
};
