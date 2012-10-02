#include "stdafx.h"
#include "tile_provider2.h"
#include "common/performance_counter.h"

int amain(int argc, char* argv[])
{
    tile_provider2 provider("192.168.121.129");

    PerformaceCounter perf_counter;
    shared_ptr<const tile_t> tile = provider.request_tile(tile_id_t(8, 127, 171));
    while (!tile->ready())
    {

    }
    std::cout << "Time: " << perf_counter.time_ms() << std::endl;
    return 0;
}

int main(int argc, char* argv[])
{
    
    
    typedef shared_ptr<const tile_t> tile_ptr_t;
    tile_provider2 provider("192.168.121.129");
    
    const tile_id_t id(5, 4, 8);

    std::cout << "Loading tile for the first time" << std::endl;

    shared_ptr<const tile_t> tile = provider.request_tile(id);

    while (!tile->ready())
    {

    }

    std::cout << "Tile ready" << std::endl;
    
    vector<tile_ptr_t> tiles;
    PerformaceCounter perf_counter;

    for (int i = 0; i < 100; ++i)
        tiles.push_back(provider.request_tile(id));

    vector<bool> ready_bits(tiles.size(), false);

    auto check_not_ready = [](const tile_ptr_t &tile) -> bool
    {
        return !(tile->ready());
    };

    int counter = 0;
    while (std::count_if(tiles.begin(), tiles.end(), check_not_ready) != 0)
    {
        for (size_t i = 0; i < tiles.size(); ++i)
        {
            if (ready_bits[i])
                continue;

            if (tiles[i]->ready())
            {
                //std::cout << "Tile " << i << " ready" << std::endl;
                ready_bits[i] = true;
            }
        }
    }
    std::cout << "Avg time: " << perf_counter.time_ms() / tiles.size() << std::endl;

    return 0;
}

