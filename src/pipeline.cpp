#include "pipeline.h"
#include "io.h"
#include "colour_converter.h"
#include <iostream>

namespace hardware
{
    namespace pipeline
    {

        Pipeline::Pipeline()
        {
            LOG_INFO("Pipeline constructor");
        }

        Pipeline::~Pipeline()
        {
            clearStages();
            LOG_INFO("Pipeline destructor");
        }

        void Pipeline::addStage(hardware::filters::BaseFilter *filter)
        {
            if (filter)
            {
                stages.push_back(filter);
                LOG_INFO("Added filter stage (total: " << stages.size() << ")");
            }
            else
            {
                LOG_ERROR("Attempted to add null filter");
            }
        }

        void Pipeline::clearStages()
        {
            LOG_INFO("Clearing " << stages.size() << " stages");
            for (size_t i = 0; i < stages.size(); i++)
            {
                if (stages[i])
                {
                    LOG_VERBOSE("Deleting stage " << i);
                    delete stages[i];
                    stages[i] = nullptr;
                }
            }
            stages.clear();
            LOG_INFO("All stages cleared");
        }

        bool Pipeline::run(const char *inputPath, const char *outputPath)
        {
            LOG_INFO("Pipeline run started");

            FrameReader reader;
            FrameWriter writer;

            int width = 0, height = 0;
            pixel *frame = reader.loadImage(inputPath, width, height);

            if (!frame)
            {
                LOG_ERROR("Failed to load image");
                return false;
            }

            LOG_INFO("Image loaded: " << width << "x" << height);

            convertToGrayscale(frame, width, height);

            pixel *input = frame;
            pixel *output = new pixel[width * height];

            if (!output)
            {
                LOG_ERROR("Failed to allocate output buffer");
                delete[] frame;
                return false;
            }

            for (auto stage : stages)
            {
                stage->apply(input, output, width, height);
                std::swap(input, output);
            }

            bool saveSuccess = writer.saveImage(outputPath, input, width, height);

            delete[] frame;
            delete[] output;

            return saveSuccess;
        }

    } // namespace pipeline
} // namespace hardware
