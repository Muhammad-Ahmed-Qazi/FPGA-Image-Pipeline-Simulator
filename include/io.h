#ifndef IO_H
#define IO_H
#include "pixel.h"
#include <string> // Add this

namespace hardware
{
    namespace pipeline
    {
        struct FrameReader
        {
            pixel *loadImage(const char *filename, int &width, int &height);
        };

        struct FrameWriter
        {
            bool saveImage(const char *filename, pixel *buffer, int width, int height);        // Changed to bool
            bool saveImage(const std::string &filename, pixel *buffer, int width, int height); // Optional overload
        };
    }
}

#endif