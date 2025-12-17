#ifndef BASE_FILTER_H
#define BASE_FILTER_H

#include "pixel.h"

namespace hardware {
    namespace filters {
        
        class BaseFilter {
        public:
            virtual void apply(pixel* input, pixel* output, int width, int height) = 0;
            virtual ~BaseFilter() = default;
        };
        
    } // namespace filters
} // namespace hardware

#endif