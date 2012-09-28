#include "stdafx.h"
#include "mypng/mypng.h"
#include "../tiles_provider/png_converter.h"
#include "common/performance_counter.h"

class my_buffer : public std::streambuf
{
public:    
    my_buffer(unsigned char *p, size_t size)
    {
        char *s = reinterpret_cast<char*>(p);
        setg(s, s, s + size);
    }
};

void libpng_converter(const unsigned char* src, unsigned char* dst, const size_t len, const size_t width, const size_t height)
{
    my_buffer buf(const_cast<unsigned char*>(src), len);
    std::istream stream(&buf);
    
    PerformaceCounter perf_count;
    png::rgb_pixel *dst1 = reinterpret_cast<png::rgb_pixel*>(dst);
    png::myimage<png::rgb_pixel> image(dst1, width, height);
    try 
    {
        image.read(stream);
        const double time_ms = perf_count.time_ms();

        qDebug() << "Png conv: " << time_ms << " ms";

/*
        unsigned char* ptr = dst;
        for (size_t y = 0; y < height; ++y)
        {
            for (size_t x = 0; x < width; ++x)
            {
                //const png::rgb_pixel pixel = image.get_pixel(x, y);
                ptr[0] = pixel.red;
                ptr[1] = pixel.green;
                ptr[2] = pixel.blue;
                ptr += 3;
            }
        }
*/
    }
    catch (png::error e)
    {
        qDebug() << "Png error: " << e.what();

    }
}

png_converter get_png_converter()
{
    return libpng_converter;
}
