#ifndef PIPELINE_H
#define PIPELINE_H

#include "config.h"
#include "base_filter.h"
#include "buffer.h"
#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

namespace hardware {
    namespace pipeline {
        
        class Pipeline {
        private:
            std::vector<filters::BaseFilter*> stages;
            hardware::memory::FrameBuffer* inputBuffer;
            hardware::memory::FrameBuffer* outputBuffer;
            
            // Function pointer registry for dynamic filter creation
            std::unordered_map<std::string, FilterCreator> filterRegistry;
            
            // Function pointer for stage callbacks
            typedef void (*StageCallback)(const char* stageName, void* userData);
            StageCallback stageCallback;
            void* callbackUserData;
            
        public:
            Pipeline();
            ~Pipeline();
            
            // Basic stage management
            void addStage(hardware::filters::BaseFilter* filter);
            
            // Dynamic filter management using function pointers
            void registerFilter(const std::string& name, FilterCreator creator);
            bool addStageByName(const std::string& filterName);
            
            // Template method for different filter types
            template<typename FilterType, typename... Args>
            void addStageT(Args&&... args) {
                stages.push_back(new FilterType(std::forward<Args>(args)...));
                LOG_INFO("Added template filter stage: " << typeid(FilterType).name());
            }
            
            // Set callback for stage notifications
            void setStageCallback(StageCallback callback, void* userData = nullptr) {
                stageCallback = callback;
                callbackUserData = userData;
            }
            
            // Pipeline execution
            bool run(const char* inputPath, const char* outputPath);
            
            // Hardware simulation methods
            #ifdef HW_SIMULATION
                void simulateClockCycles(int cycles);
                void dumpPipelineRegisters() const;
                int estimateResourceUsage() const;
            #endif
            
            // Utility methods
            void listRegisteredFilters() const;
            int getStageCount() const { return stages.size(); }
            void clearStages();
            
        private:
            void notifyStage(const char* stageName) {
                if (stageCallback) {
                    stageCallback(stageName, callbackUserData);
                }
            }
            
            bool allocateBuffers(int width, int height);
            void releaseBuffers();
        };
        
    }
}

#endif // PIPELINE_H