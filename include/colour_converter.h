#ifndef COLOUR_CONVERTER_H
#define COLOUR_CONVERTER_H

#include "pixel.h"

namespace hardware {
    namespace pipeline {
        
        void convertToGrayscale(pixel* frame, int width, int height);
        
    }
}

#endif
