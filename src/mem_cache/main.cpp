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
    cache.add(5, 5);

    return 0;
}