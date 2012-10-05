#include "stdafx.h"
#include "dumb_cache.h"
#include "../tiles_provider/lru_map.h"

template<typename T>
class ordered_generator
{
public:
    ordered_generator(const T& val = 0)
        : val_(val)
    {

    }

    T operator()()
    {
        return val_++;
    }
private:
    T val_;
};

struct my_alloc
{

};


int main()
{
    typedef lru_map<int, int> map_t;
    map_t cache;
    for (int i = 0; i < 10; ++i)
        cache.insert(make_pair(i, i * i));

    cache.request(4);
    cache.request(3);
    
    auto pred = [](const map_t::value_type &v) -> bool
    {
        return v.first % 2 == 1;
    }; 
    
    for (auto it = cache.begin(); it != cache.end(); )
    {
        if (pred(*it))
            it = cache.erase(it);
        else
            ++it;
    }
                                                     
    BOOST_FOREACH(const auto &v, cache)
    {
        cout << v.first << ": " << v.second << endl;
    }
    return 0;
}
