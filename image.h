#ifndef IMAGE_H
#define IMAGE_H
#include <cstdint>
typedef struct img{
    int32_t width;
    int32_t height;
    int32_t channels;
    unsigned char* pixels;
} img;

#endif