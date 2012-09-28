#pragma once

typedef boost::function<void(const unsigned char*, unsigned char*, size_t, size_t, size_t)> png_converter;

png_converter get_png_converter();
