#include "stdafx.h"
#include "../wms/png_converter.h"
#include "common/performance_counter.h"

void qt_png_converter(const unsigned char* src, unsigned char* dst, const size_t len, const size_t width, const size_t height)
{

    QImage img(width, height, QImage::Format_ARGB32_Premultiplied);
    PerformaceCounter perf_count;
    bool res = img.loadFromData(src, len);
    //qDebug() << perf_count.time_ms() << " ms";

/*
    unsigned char* ptr = dst;
    for (size_t y = 0; y < height; ++y)
    {
        for (size_t x = 0; x < width; ++x)
        {
            const QRgb rgb = img.pixel(x, y);
            ptr[0] = qRed(rgb);
            ptr[1] = qGreen(rgb);
            ptr[2] = qBlue(rgb);
            ptr += 3;
        }
    }
*/
    
}

