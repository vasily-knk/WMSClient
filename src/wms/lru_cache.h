#pragma once

#include "wms/png_provider.h"
#include "png_cache.h"
#include "lru_map.h"

namespace wms
{

class lru_cache : public png_provider_cache
{
public:
    typedef lru_map<tile_id_t, shared_ptr<const png_t>, tile_id_hash> png_map_t;

    lru_cache(size_t lower, size_t upper);

    void cache_png(const tile_id_t &id, shared_ptr<const png_t> png); 
    bool request_png(const tile_id_t &id, const png_callback &callback);

    size_t size() const
    {
        return size_;
    }

    const png_map_t &get_map() const
    {
        return map_;
    }

private:
    const size_t lower_, upper_;
    png_map_t map_;    
    boost::mutex mutex_;
    size_t size_;
};

} // namespace wms