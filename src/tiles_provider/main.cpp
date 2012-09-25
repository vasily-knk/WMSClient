#include "stdafx.h"
#include "tiles_provider.h"


int main(int argc, char* argv[])
{
    typedef shared_ptr<const tile_t> tile_ptr_t;
    tiles_provider provider;
    vector<tile_ptr_t> tiles;
    
    for (int i = 0; i < 10; ++i)
        tiles.push_back(provider.request_tile(tile_id_t(0, 0, 0)));

    vector<bool> ready_bits(tiles.size(), false);

    auto check_not_ready = [](const tile_ptr_t &tile) -> bool
    {
        return !(tile->ready());
    };

    while (std::count_if(tiles.begin(), tiles.end(), check_not_ready) != 0)
    {
        for (size_t i = 0; i < tiles.size(); ++i)
        {
            if (ready_bits[i])
                continue;

            if (tiles[i]->ready())
            {
                std::cout << "Tile " << i << " ready" << std::endl;
                ready_bits[i] = true;
            }
        }
    }

    return 0;
}

