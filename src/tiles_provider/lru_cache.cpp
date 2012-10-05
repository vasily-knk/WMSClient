#include "stdafx.h"
#include "lru_cache.h"

lru_cache::lru_cache(const size_t lower, const size_t upper)
    : lower_(lower)
    , upper_(upper)
{
    if (lower_ > upper_)
        throw std::runtime_error("lru_cache: lower bound is larger then upper bound");
}

void lru_cache::cache_png(const tile_id_t &id, shared_ptr<const png_t> png)
{
    boost::lock_guard<boost::mutex> lock(mutex_);

    map_.insert(make_pair(id, png));
    if (map_.size() <= upper_)
        return;

    for (auto it = map_.rbegin(); it != map_.rend() && map_.size() > lower_;)
    {
        auto temp = map_.erase(--it.base());
        it = png_map_t::reverse_iterator(temp);
    }
}

bool lru_cache::request_png(const tile_id_t &id, const png_callback &callback)
{
    boost::lock_guard<boost::mutex> lock(mutex_);

    if (map_.count(id) == 0)
        return false;

    auto png = map_.request(id);

    callback(png);
    return true;
}
