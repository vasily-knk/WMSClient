#include "stdafx.h"
#include "dummy_cache.h"

dummy_cache::dummy_cache()
    : size_(0)
{

}

void dummy_cache::cache_png(const tile_id_t &id, shared_ptr<const png_t> png)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);

    if (map_.count(id) != 0)
        return;

    map_.insert(make_pair(id, png));
    size_ += png->size();
}

bool dummy_cache::request_png(const tile_id_t &id, const png_callback &callback)
{
    boost::lock_guard<boost::recursive_mutex> lock(mutex_);

    auto it = map_.find(id);
    if (it == map_.end())
        return false;

    callback(it->second);
    return true;
}
