#pragma once

#include "tile.h"

class tile_loader
{
public:
    virtual shared_ptr<const tile_t> request_tile(const tile_id_t &id) = 0;
};
