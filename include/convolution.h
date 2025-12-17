#ifndef CONVOLUTION_H
#define CONVOLUTION_H

#include "base_filter.h"
#include "pixel.h"
#include <vector>
#include <cmath>
#include <cstdint>

namespace hardware
{
    namespace filters
    {
        // Simple clamp helper
        template <typename T>
        T clamp_value(T val, T min_val, T max_val)
        {
            return (val < min_val) ? min_val : (val > max_val) ? max_val
                                                               : val;
        }

        class ConvolutionFilter : public BaseFilter
        {
        private:
            std::vector<float> kernel;
            int kernelSize;
            int kernelRadius;

        public:
            ConvolutionFilter(const std::vector<float> &k, int size);

            static ConvolutionFilter *createGaussian(int size, float sigma);
            static ConvolutionFilter *createSharpen();
            static ConvolutionFilter *createSobelX();
            static ConvolutionFilter *createSobelY();

            void apply(pixel *input, pixel *output, int width, int height) override;

            void printKernel() const;
            int getKernelSize() const { return kernelSize; }
        };

        // Template version for compile-time kernel sizes
        template <int KERNEL_SIZE>
        class TemplateConvolutionFilter : public BaseFilter
        {
        private:
            float kernel[KERNEL_SIZE][KERNEL_SIZE];

        public:
            TemplateConvolutionFilter(const float k[KERNEL_SIZE][KERNEL_SIZE])
            {
                for (int i = 0; i < KERNEL_SIZE; i++)
                {
                    for (int j = 0; j < KERNEL_SIZE; j++)
                    {
                        kernel[i][j] = k[i][j];
                    }
                }
            }

            void apply(pixel *input, pixel *output, int width, int height) override
            {
                int radius = KERNEL_SIZE / 2;

                for (int y = radius; y < height - radius; y++)
                {
                    for (int x = radius; x < width - radius; x++)
                    {
                        float sum_r = 0.0f, sum_g = 0.0f, sum_b = 0.0f;

                        for (int ky = -radius; ky <= radius; ky++)
                        {
                            for (int kx = -radius; kx <= radius; kx++)
                            {
                                int idx = (y + ky) * width + (x + kx);
                                float weight = kernel[ky + radius][kx + radius];

                                sum_r += input[idx].r * weight;
                                sum_g += input[idx].g * weight;
                                sum_b += input[idx].b * weight;
                            }
                        }

                        // Clamp and store (using custom clamp)
                        int idx = y * width + x;
                        output[idx].r = static_cast<uint8_t>(clamp_value(sum_r, 0.0f, 255.0f));
                        output[idx].g = static_cast<uint8_t>(clamp_value(sum_g, 0.0f, 255.0f));
                        output[idx].b = static_cast<uint8_t>(clamp_value(sum_b, 0.0f, 255.0f));
                    }
                }

                // Handle borders
                for (int y = 0; y < height; y++)
                {
                    for (int x = 0; x < width; x++)
                    {
                        if (y < radius || y >= height - radius ||
                            x < radius || x >= width - radius)
                        {
                            output[y * width + x] = input[y * width + x];
                        }
                    }
                }
            }
        };
    }
}

#endif