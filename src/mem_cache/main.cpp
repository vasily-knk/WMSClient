#include "stdafx.h"
#include "dumb_cache.h"

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
    dumb_cache<int, int> cache;
    for (int i = 0; i < 10; ++i)
        cache.add(i, i);

    if (cache.contains(5))
        cache.request(5);

    BOOST_FOREACH(int i, cache)
        cout << i << endl;
    return 0;
}