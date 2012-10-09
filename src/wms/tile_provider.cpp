#include "wms/tile_provider.h"
#include "lru_cache.h"
#include "wms_png_provider.h"

namespace wms
{

tile_provider::tile_provider(const string &wms_host, const string &wms_path, size_t cache_lower_bound, size_t cache_upper_bound)
/*
    : cache_()
    , wms_(new wms_png_provider(wms_host, wms_path, io_service()))
*/
{
    add_provider(shared_ptr<png_provider>(new lru_cache(cache_lower_bound, cache_upper_bound)));
    add_provider(shared_ptr<png_provider>(new wms_png_provider(wms_host, wms_path, io_service())));
}

} // namespace wms
