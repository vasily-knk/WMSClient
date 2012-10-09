#pragma once

#include "tile_provider_base.h"

namespace wms
{

class tile_provider : public tile_provider_base
{
public:
    tile_provider(const string &wms_host, const string &wms_path, size_t cache_lower_bound, size_t cache_upper_bound);
private:
};

} // namespace wms
