#include "edge_filter.h"
#include "fixed_point.h"
#include <cstdlib>
#include <algorithm>
#include <iostream>

namespace hardware
{
    namespace filters
    {
        EdgeFilter::EdgeFilter()
        {
            std::cout << "[EDGE] EdgeFilter constructor called" << std::endl;
        }

        EdgeFilter::~EdgeFilter()
        {
            std::cout << "[EDGE] EdgeFilter destructor called" << std::endl;
        }

        void EdgeFilter::apply(pixel *input, pixel *output, int width, int height)
        {
            std::cout << "[EDGE] apply() called. Width: " << width << ", Height: " << height << std::endl;

            // Validate parameters
            if (!input || !output)
            {
                std::cerr << "[EDGE] ERROR: Null pointer input/output!" << std::endl;
                return;
            }

            if (width <= 2 || height <= 2)
            {
                std::cerr << "[EDGE] ERROR: Image too small for filter!" << std::endl;
                // Set all pixels to black
                for (int i = 0; i < width * height; i++)
                {
                    output[i] = pixel{0, 0, 0};
                }
                return;
            }

            int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
            int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

            std::cout << "[EDGE] Processing interior pixels..." << std::endl;

            for (int y = 1; y < height - 1; y++)
            {
                for (int x = 1; x < width - 1; x++)
                {
                    Fixed gx = 0, gy = 0;
                    for (int ky = -1; ky <= 1; ky++)
                    {
                        for (int kx = -1; kx <= 1; kx++)
                        {
                            int idx = (y + ky) * width + (x + kx);

                            // Bounds check
                            if (idx < 0 || idx >= width * height)
                            {
                                std::cerr << "[EDGE] ERROR: Index out of bounds! idx=" << idx << std::endl;
                                continue;
                            }

                            Fixed intensity = Fixed((float)input[idx].r);
                            gx += intensity * Gx[ky + 1][kx + 1];
                            gy += intensity * Gy[ky + 1][kx + 1];
                        }
                    }

                    int mag = (int)(float(std::abs(gx) + std::abs(gy)));
                    if (mag > 255)
                        mag = 255;

                    int idx = y * width + x;

                    // Bounds check for output
                    if (idx < 0 || idx >= width * height)
                    {
                        std::cerr << "[EDGE] ERROR: Output index out of bounds!" << std::endl;
                        continue;
                    }

                    output[idx].r = output[idx].g = output[idx].b = (uint8_t)mag;
                }
            }

            std::cout << "[EDGE] Setting borders to black..." << std::endl;

            // Handle borders - set to black
            for (int x = 0; x < width; x++)
            {
                if (x < width * height)
                    output[x] = pixel{0, 0, 0};
                if ((height - 1) * width + x < width * height)
                {
                    output[(height - 1) * width + x] = pixel{0, 0, 0};
                }
            }

            for (int y = 0; y < height; y++)
            {
                if (y * width < width * height)
                    output[y * width] = pixel{0, 0, 0};
                if (y * width + width - 1 < width * height)
                {
                    output[y * width + width - 1] = pixel{0, 0, 0};
                }
            }

            std::cout << "[EDGE] Filter applied successfully." << std::endl;
        }
    }
}