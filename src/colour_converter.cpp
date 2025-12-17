#include "colour_converter.h"
#include <cstdint>

namespace hardware
{
    namespace pipeline
    {
        void convertToGrayscale(pixel *frame, int width, int height)
        {
            for (int i = 0; i < width * height; i++)
            {
                uint8_t gray = static_cast<uint8_t>(
                    0.299f * frame[i].r +
                    0.587f * frame[i].g +
                    0.114f * frame[i].b);
                frame[i].r = frame[i].g = frame[i].b = gray;
            }
        }
    }
}