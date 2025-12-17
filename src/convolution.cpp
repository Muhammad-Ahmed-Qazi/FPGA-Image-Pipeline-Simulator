#include "convolution.h"
#include "fixed_point.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include <cstdint>
#include "config.h"

#ifdef USE_FIXED_POINT
#define PROCESS_VALUE(val, weight) (Fixed(val) * Fixed(weight))
#define CONVERT_BACK(val) float(val)
#else
#define PROCESS_VALUE(val, weight) ((val) * (weight))
#define CONVERT_BACK(val) static_cast<uint8_t>(clamp_value((val), 0.0f, 255.0f))
#endif

namespace hardware
{
    namespace filters
    {
        ConvolutionFilter::ConvolutionFilter(const std::vector<float> &k, int size)
            : kernel(k), kernelSize(size), kernelRadius(size / 2)
        {
        }

        ConvolutionFilter *ConvolutionFilter::createGaussian(int size, float sigma)
        {
            std::vector<float> kernel(size * size);
            int radius = size / 2;
            float sum = 0.0f;

            // Create Gaussian kernel
            for (int y = -radius; y <= radius; y++)
            {
                for (int x = -radius; x <= radius; x++)
                {
                    float value = exp(-(x * x + y * y) / (2 * sigma * sigma)) / (2 * M_PI * sigma * sigma);
                    kernel[(y + radius) * size + (x + radius)] = value;
                    sum += value;
                }
            }

            // Normalize
            for (auto &val : kernel)
            {
                val /= sum;
            }

            return new ConvolutionFilter(kernel, size);
        }

        ConvolutionFilter *ConvolutionFilter::createSharpen()
        {
            // Sharpening kernel
            std::vector<float> kernel = {
                0, -1, 0,
                -1, 5, -1,
                0, -1, 0};
            return new ConvolutionFilter(kernel, 3);
        }

        ConvolutionFilter *ConvolutionFilter::createSobelX()
        {
            std::vector<float> kernel = {
                -1, 0, 1,
                -2, 0, 2,
                -1, 0, 1};
            return new ConvolutionFilter(kernel, 3);
        }

        ConvolutionFilter *ConvolutionFilter::createSobelY()
        {
            std::vector<float> kernel = {
                -1, -2, -1,
                0, 0, 0,
                1, 2, 1};
            return new ConvolutionFilter(kernel, 3);
        }

        void ConvolutionFilter::apply(pixel *input, pixel *output, int width, int height)
        {

// Hardware simulation: register for accumulation
#ifdef DEBUG
            std::cout << "[CONV] Applying " << kernelSize << "x" << kernelSize
                      << " convolution\n";
            std::cout << "[CONV] Kernel radius: " << kernelRadius << "\n";
#endif

            for (int y = kernelRadius; y < height - kernelRadius; y++)
            {
                for (int x = kernelRadius; x < width - kernelRadius; x++)
                {
// Simulated hardware registers
#ifdef USE_FIXED_POINT
                    Fixed sum_r = 0, sum_g = 0, sum_b = 0;
#else
                    float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;
#endif

                    for (int ky = -kernelRadius; ky <= kernelRadius; ky++)
                    {
                        for (int kx = -kernelRadius; kx <= kernelRadius; kx++)
                        {
                            int idx = (y + ky) * width + (x + kx);
                            int kernelIdx = (ky + kernelRadius) * kernelSize + (kx + kernelRadius);

#ifdef USE_FIXED_POINT
                            Fixed weight = Fixed(kernel[kernelIdx]);
                            sum_r += PROCESS_VALUE(input[idx].r, weight);
                            sum_g += PROCESS_VALUE(input[idx].g, weight);
                            sum_b += PROCESS_VALUE(input[idx].b, weight);
#else
                            sum_r += PROCESS_VALUE(input[idx].r, kernel[kernelIdx]);
                            sum_g += PROCESS_VALUE(input[idx].g, kernel[kernelIdx]);
                            sum_b += PROCESS_VALUE(input[idx].b, kernel[kernelIdx]);
#endif
                        }
                    }

                    int idx = y * width + x;
                    output[idx].r = CONVERT_BACK(sum_r);
                    output[idx].g = CONVERT_BACK(sum_g);
                    output[idx].b = CONVERT_BACK(sum_b);

#ifdef DEBUG
                    if (x == kernelRadius && y == kernelRadius)
                    {
                        std::cout << "[CONV] First pixel result: "
                                  << (int)output[idx].r << "\n";
                    }
#endif
                }
            }

            // Handle borders by copying
            for (int y = 0; y < height; y++)
            {
                for (int x = 0; x < width; x++)
                {
                    if (y < kernelRadius || y >= height - kernelRadius ||
                        x < kernelRadius || x >= width - kernelRadius)
                    {
                        output[y * width + x] = input[y * width + x];
                    }
                }
            }
        }

        void ConvolutionFilter::printKernel() const
        {
            std::cout << "Convolution Kernel " << kernelSize << "x" << kernelSize << ":\n";
            for (int i = 0; i < kernelSize; i++)
            {
                for (int j = 0; j < kernelSize; j++)
                {
                    std::cout << kernel[i * kernelSize + j] << "\t";
                }
                std::cout << "\n";
            }
        }
    }
}