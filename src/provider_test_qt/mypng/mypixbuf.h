#pragma once

namespace png
{
    template<typename P>
    class my_pixel_row
    {
    public:
        //friend class my_row_traits;
        typedef P pixel;
    public:
        my_pixel_row(pixel *data, size_t size)
            : data_(data)
            , size_(size)
        {

        }
    
    public:
        pixel *data_;
        size_t size_;
    };

    template<typename P>
    class my_row_traits
    {
    public:
        typedef P pixel;
        static pixel* get_data(my_pixel_row<pixel>& row)
        {
            return row.data_;
        }
    };
    
    template<typename P>
    class my_pixel_buffer
    {
    public:
        typedef P pixel;
        typedef typename my_pixel_row<pixel> row_type;
        typedef typename my_row_traits<pixel> row_traits;

        my_pixel_buffer(pixel *data, size_t width, size_t height)
            : data_(data)
            , width_(width)
            , height_(height)
        {

        }

        row_type get_row(size_t index)
        {
            return row_type(data_ + height_ * index, width_);
        }

    private:
        pixel *data_;
        size_t width_, height_;
    };
}