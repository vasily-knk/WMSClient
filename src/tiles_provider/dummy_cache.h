#pragma once

#include "png_cache.h"
#include "png_provider.h"

class dummy_cache : public png_cache, public png_provider
{
public:
    dummy_cache();

    void cache_png(const tile_id_t &id, shared_ptr<const png_t> png); 
    bool request_png(const tile_id_t &id, const png_callback &callback);

    size_t size() const
    {
        return size_;
    }

private:
    typedef unordered_map<tile_id_t, shared_ptr<const png_t>> png_map_t;
    png_map_t map_;    
    boost::recursive_mutex mutex_;
    size_t size_;
};
