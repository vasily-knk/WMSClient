#pragma once

struct tile_id_t
{
    tile_id_t(int zoom, int x, int y)
        : zoom(zoom)
        , x(x)
        , y(y)
    {

    }

    int zoom, x, y;
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
    explicit tile_t()
        : data_(new data_ptr_t[WIDTH * HEIGHT * BYTES_PER_PIXEL])
        , ready_(false)
    {

    }

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

private:
    scoped_array<data_ptr_t> data_;
    bool ready_;
    mutable boost::mutex mutex_;
};
