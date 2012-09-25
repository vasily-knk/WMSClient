#include "stdafx.h"
#include "dummy_tile_loader.h"

dummy_tile_loader::dummy_tile_loader(boost::asio::io_service &io)
    : io_(&io)
{

}

void dummy_tile_loader::callback(const shared_ptr<tile_t> &dst, const shared_ptr<boost::asio::deadline_timer> &timer)
{
    dst->set_ready(true);
}

shared_ptr<const tile_t> dummy_tile_loader::request_tile(const tile_id_t &id)
{
    shared_ptr<tile_t> tile(new tile_t);
    fill_tile(*tile);

    shared_ptr<timer_t> timer(new timer_t(*io_, boost::posix_time::milliseconds(rand() % 3000)));

    auto f = boost::bind(&dummy_tile_loader::callback, this, tile, timer);
    timer->async_wait(f);

    return tile;
}

void dummy_tile_loader::fill_tile(tile_t &tile)
{
    const unsigned char r = rand() & 0xFF;
    const unsigned char g = rand() & 0xFF;
    const unsigned char b = rand() & 0xFF;

    tile_t::data_ptr_t *ptr = tile.get_data();
    for (size_t y = 0; y < tile_t::HEIGHT; ++y)
    {
        for (size_t x = 0; x < tile_t::WIDTH; ++x)
        {
            ptr[0] = r; 
            ptr[1] = g; 
            ptr[2] = b; 
            ptr += tile_t::BYTES_PER_PIXEL;
        }
    }
}
