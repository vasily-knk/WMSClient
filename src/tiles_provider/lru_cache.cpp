#include "stdafx.h"
#include "lru_cache.h"

lru_cache::lru_cache(const size_t max_size)
    : max_size_(max_size)
    , size_(0)
{

}

void lru_cache::cache_png(const tile_id_t &id, shared_ptr<const png_t> png)
{
    boost::lock_guard<boost::mutex> lock(mutex_);

    map_.add(id, png);
    ++size_;
}

bool lru_cache::request_png(const tile_id_t &id, const png_callback &callback)
{
    boost::lock_guard<boost::mutex> lock(mutex_);

    if (!map_.contains(id))
        return false;

    auto png = map_.request(id);

    callback(png);
    return true;
}
