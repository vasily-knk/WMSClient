#pragma once

namespace wms 
{

struct tile_id_t
{
    tile_id_t(int zoom, int x, int y)
        : zoom(zoom)
        , x(x)
        , y(y)
    {

    }

    bool operator==(const tile_id_t &other) const
    {
        return (zoom == other.zoom && x == other.x && y == other.y);
    }

    int zoom, x, y;
};

class tile_id_hash
{
public:
    size_t operator() (const wms::tile_id_t &id) const
    {
        using boost::hash_combine;

        size_t seed = 0;
        hash_combine(seed, id.zoom);
        hash_combine(seed, id.x);
        hash_combine(seed, id.y);
        return seed;
    }
};

class tile_t
{
public:
    const static size_t WIDTH = 256;
    const static size_t HEIGHT = 256;
    const static size_t BYTES_PER_PIXEL = 3;
    typedef unsigned char data_ptr_t;

public:
    friend class tiles_provider;

public:
    tile_t()
        : data_(new data_ptr_t[WIDTH * HEIGHT * BYTES_PER_PIXEL])
        , ready_(false)
    {
        ++num_tiles_;
    }
    
    ~tile_t()
    {
        --num_tiles_;
    }
private:
    tile_t(const tile_t&);

public:
    data_ptr_t *get_data() const
    {
        return data_.get();
    }

    bool ready() const
    {
        boost::lock_guard<boost::mutex> lock(mutex_);
        return ready_;
    }

    void set_ready(const bool value)
    {
        boost::lock_guard<boost::mutex> lock(mutex_);
        ready_ = value;
    }

public:
    static size_t get_num_tiles() 
    {
        return num_tiles_;
    }

private:
    scoped_array<data_ptr_t> data_;
    bool ready_;
    mutable boost::mutex mutex_;
    
    static size_t num_tiles_;
};

typedef vector<unsigned char> png_t;

} // namespace wms

/*
namespace boost
{
    inline size_t hash_value(const wms::tile_id_t &id)
    {
        size_t seed = 0;
        hash_combine(seed, id.zoom);
        hash_combine(seed, id.x);
        hash_combine(seed, id.y);
        return seed;
    }
}
*/
