#ifndef SMOOTHING_FILTER_H
#define SMOOTHING_FILTER_H

#include "base_filter.h"

namespace hardware
{
	namespace filters
	{
		class SmoothingFilter : public BaseFilter
		{
		public:
			void apply(pixel *input, pixel *output, int width, int height) override;
		};
	}
}

#endif
