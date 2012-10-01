#pragma once

#include "tile.h"

//class png_t;

struct png_provider
{
    typedef boost::function<void(shared_ptr<const png_t>)> png_callback;
    virtual bool request_png(const tile_id_t &id, const png_callback &callback) = 0;
};