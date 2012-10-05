#pragma once

#include "png_cache.h"
#include "png_provider.h"
#include "lru_map.h"

class lru_cache : public png_cache, public png_provider
{
public:
    typedef lru_map<tile_id_t, shared_ptr<const png_t>> png_map_t;

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
