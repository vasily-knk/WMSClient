#pragma once

#include "tile.h"

class png_cache
{
public:
    virtual void cache_png(const tile_id_t &id, shared_ptr<const png_t> png) = 0;
};
