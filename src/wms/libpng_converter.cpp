#include "mypng/mypng.h"
#include "wms/png_converter.h"
#include "common/performance_counter.h"

namespace wms 
{

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
    
    png::rgb_pixel *dst1 = reinterpret_cast<png::rgb_pixel*>(dst);
    png::myimage<png::rgb_pixel> image(dst1, width, height);
    try 
    {
        image.read(stream);
    }
    catch (png::error e)
    {
        // TODO: Error handling
    }
}

png_converter get_png_converter()
{
    return libpng_converter;
}

} // namespace wms

