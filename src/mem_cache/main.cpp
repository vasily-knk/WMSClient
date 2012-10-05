#include "stdafx.h"
#include "dumb_cache.h"
#include "lru_map.h"

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
    lru_map<int, int> cache;
    for (int i = 0; i < 10; ++i)
        cache.insert(make_pair(i, i * i));

    cache.request(5);
    cache.request(3);

    BOOST_FOREACH(auto &v, cache)
    {
        ++v.second;
        cout << v.first << ": " << v.second << endl;
    }
    return 0;
}
