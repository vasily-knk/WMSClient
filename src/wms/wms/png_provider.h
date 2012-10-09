#pragma once

#include "tile.h"

namespace wms
{

struct png_provider
{
    typedef boost::function<void(shared_ptr<const png_t>)> png_callback;
    virtual bool request_png(const tile_id_t &id, const png_callback &callback) = 0;
    virtual void cache_png(const tile_id_t &id, shared_ptr<const png_t> png) = 0;
};

} // namespace wms
