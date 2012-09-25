#include "stdafx.h"
#include "tiles_provider.h"

int main(int argc, char* argv[])
{
    tiles_provider provider;
    shared_ptr<const tile_t> tile = provider.request_tile(tile_id_t(8, 127, 171));
    while (!tile->ready())
    {

    }
    return 0;
}

int amain(int argc, char* argv[])
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

    int counter = 0;
    while (std::count_if(tiles.begin(), tiles.end(), check_not_ready) != 0)
    {
        boost::this_thread::sleep(boost::posix_time::seconds(1));
        std::cout << "working " << std::endl;

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

