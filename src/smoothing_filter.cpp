#include "smoothing_filter.h"
#include "fixed_point.h"
#include <cstdint>
#include <iostream>

namespace hardware
{
    namespace filters
    {
        void SmoothingFilter::apply(pixel *input, pixel *output, int width, int height)
        {
            std::cout << "[SMOOTH] apply() called. Width: " << width << ", Height: " << height << std::endl;

            // Validate parameters
            if (!input || !output)
            {
                std::cerr << "[SMOOTH] ERROR: Null pointer input/output!" << std::endl;
                return;
            }

            if (width <= 2 || height <= 2)
            {
                std::cerr << "[SMOOTH] ERROR: Image too small for filter!" << std::endl;
                // Just copy input to output
                for (int i = 0; i < width * height; i++)
                {
                    output[i] = input[i];
                }
                return;
            }

            std::cout << "[SMOOTH] Processing interior pixels..." << std::endl;

            for (int y = 1; y < height - 1; y++)
            {
                for (int x = 1; x < width - 1; x++)
                {
                    Fixed sum = 0;
                    for (int dy = -1; dy <= 1; dy++)
                    {
                        for (int dx = -1; dx <= 1; dx++)
                        {
                            int idx = (y + dy) * width + (x + dx);
                            // Debug bounds check
                            if (idx < 0 || idx >= width * height)
                            {
                                std::cerr << "[SMOOTH] ERROR: Index out of bounds! idx=" << idx
                                          << " max=" << (width * height - 1) << std::endl;
                                continue;
                            }
                            sum += Fixed((float)input[idx].r);
                        }
                    }

                    uint8_t avg = (uint8_t)(float(sum) / 9.0f);
                    int idx = y * width + x;

                    // Debug bounds check for output
                    if (idx < 0 || idx >= width * height)
                    {
                        std::cerr << "[SMOOTH] ERROR: Output index out of bounds!" << std::endl;
                        continue;
                    }

                    output[idx].r = output[idx].g = output[idx].b = avg;
                }
            }

            std::cout << "[SMOOTH] Copying borders..." << std::endl;

            // Borders - top and bottom
            for (int x = 0; x < width; x++)
            {
                if (x < width * height)
                    output[x] = input[x];
                if ((height - 1) * width + x < width * height)
                {
                    output[(height - 1) * width + x] = input[(height - 1) * width + x];
                }
            }

            // Borders - left and right
            for (int y = 0; y < height; y++)
            {
                if (y * width < width * height)
                    output[y * width] = input[y * width];
                if (y * width + width - 1 < width * height)
                {
                    output[y * width + width - 1] = input[y * width + width - 1];
                }
            }

            std::cout << "[SMOOTH] Filter applied successfully." << std::endl;
        }
    }
}