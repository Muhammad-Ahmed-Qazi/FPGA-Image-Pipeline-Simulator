#ifndef EDGE_FILTER_H
#define EDGE_FILTER_H

#include "base_filter.h"
#include "convolution.h" // Include convolution

namespace hardware
{
    namespace filters
    {
        class EdgeFilter : public BaseFilter
        {
        private:
            ConvolutionFilter *sobelX;
            ConvolutionFilter *sobelY;

        public:
            EdgeFilter();
            ~EdgeFilter();
            void apply(pixel *input, pixel *output, int width, int height) override;
        };
    }
}

#endif