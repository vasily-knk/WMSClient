#pragma once

#include "tile.h"

class png_cache
{
public:
    virtual void cache_png(const tile_id_t *id, const tile_t::data_ptr_t *data) = 0;
};
