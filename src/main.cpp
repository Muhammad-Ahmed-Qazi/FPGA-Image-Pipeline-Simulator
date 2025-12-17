#include "pipeline.h"
#include "smoothing_filter.h"
#include "edge_filter.h"
#include "convolution.h"
#include "config.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstring>

// Using declarations
using hardware::pipeline::Pipeline;
using hardware::filters::SmoothingFilter;
using hardware::filters::EdgeFilter;
using hardware::filters::ConvolutionFilter;

void printUsage(const char* programName) {
    std::cout << "FPGA Image Processing Pipeline Simulator\n";
    std::cout << "=========================================\n";
    std::cout << "Usage: " << programName << " <input.ppm> <output.ppm> [options]\n";
    std::cout << "\nOptions:\n";
    std::cout << "  --mode=basic     : Smoothing -> Edge Detection (default)\n";
    std::cout << "  --mode=conv      : Gaussian Blur -> Sharpen\n";
    std::cout << "  --mode=all       : Run all pipelines\n";
    std::cout << "  --help, -h       : Show this help\n";
    std::cout << "\nExamples:\n";
    std::cout << "  " << programName << " input.ppm output.ppm\n";
    std::cout << "  " << programName << " input.ppm output.ppm --mode=conv\n";
    std::cout << "  " << programName << " input.ppm output.ppm --mode=all\n";
}

int main(int argc, char* argv[]) {
    // Handle help flag first (should succeed even with no other args)
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            printUsage(argv[0]);
            return 0;  // SUCCESS exit code for help
        }
    }
    
    // Check for minimum required arguments
    if (argc < 3) {
        std::cerr << "Error: Missing required arguments\n\n";
        printUsage(argv[0]);
        return 1;  // ERROR exit code for missing args
    }
    
    std::string inputPath = argv[1];
    std::string outputPath = argv[2];
    std::string mode = "basic";
    
    // Parse additional arguments
    for (int i = 3; i < argc; i++) {
        if (strncmp(argv[i], "--mode=", 7) == 0) {
            mode = argv[i] + 7;
        } else if (strcmp(argv[i], "--help") == 0 || strcmp(argv[i], "-h") == 0) {
            // Already handled, but keep for consistency
            printUsage(argv[0]);
            return 0;
        } else {
            std::cerr << "Warning: Unknown argument '" << argv[i] << "'\n";
        }
    }
    
    LOG_INFO("Starting FPGA Image Processing Pipeline");
    LOG_INFO("Input: " << inputPath);
    LOG_INFO("Output: " << outputPath);
    LOG_INFO("Mode: " << mode);
    
    bool success = false;
    int pipelinesCompleted = 0;
    
    // Basic pipeline: Smoothing -> Edge Detection
    if (mode == "basic" || mode == "all") {
        LOG_INFO("Running: Smoothing -> Edge Detection");
        
        Pipeline pipeline1;
        pipeline1.addStage(new SmoothingFilter());
        pipeline1.addStage(new EdgeFilter());
        
        std::string out1 = outputPath;
        if (mode == "all") {
            size_t dot = outputPath.find_last_of('.');
            if (dot != std::string::npos) {
                out1 = outputPath.substr(0, dot) + "_basic.ppm";
            } else {
                out1 = outputPath + "_basic.ppm";
            }
        }
        
        if (pipeline1.run(inputPath.c_str(), out1.c_str())) {
            LOG_INFO("Basic pipeline complete: " << out1);
            pipelinesCompleted++;
            success = true;
        } else {
            LOG_ERROR("Basic pipeline failed");
        }
    }
    
    // Convolution pipeline: Gaussian Blur -> Sharpen
    if (mode == "conv" || mode == "all") {
        LOG_INFO("Running: Gaussian Blur -> Sharpen");
        
        Pipeline pipeline2;
        
        // Check if convolution is available
        #ifdef HAS_CONVOLUTION
            auto gaussian = ConvolutionFilter::createGaussian(5, 1.0f);
            auto sharpen = ConvolutionFilter::createSharpen();
            
            if (gaussian && sharpen) {
                pipeline2.addStage(gaussian);
                pipeline2.addStage(sharpen);
                
                std::string out2 = outputPath;
                if (mode == "all") {
                    size_t dot = outputPath.find_last_of('.');
                    if (dot != std::string::npos) {
                        out2 = outputPath.substr(0, dot) + "_conv.ppm";
                    } else {
                        out2 = outputPath + "_conv.ppm";
                    }
                }
                
                if (pipeline2.run(inputPath.c_str(), out2.c_str())) {
                    LOG_INFO("Convolution pipeline complete: " << out2);
                    pipelinesCompleted++;
                    success = true;
                }
                
                // Pipeline owns the filters, don't delete manually
            } else {
                LOG_ERROR("Failed to create convolution filters");
                if (gaussian) delete gaussian;
                if (sharpen) delete sharpen;
            }
        #else
            // Fallback to basic filters if convolution not available
            LOG_WARNING("Convolution not available, using smoothing as fallback");
            pipeline2.addStage(new SmoothingFilter());
            pipeline2.addStage(new SmoothingFilter());  // Second smoothing as simple blur
            
            std::string out2 = outputPath;
            if (mode == "all") {
                size_t dot = outputPath.find_last_of('.');
                out2 = outputPath.substr(0, dot) + "_conv.ppm";
            }
            
            if (pipeline2.run(inputPath.c_str(), out2.c_str())) {
                LOG_INFO("Fallback convolution pipeline complete: " << out2);
                pipelinesCompleted++;
                success = true;
            }
        #endif
    }
    
    // Final status
    if (success && pipelinesCompleted > 0) {
        LOG_INFO("Successfully completed " << pipelinesCompleted << " pipeline(s)");
        std::cout << "\n=========================================\n";
        std::cout << "SUCCESS: Pipeline execution completed\n";
        std::cout << "Pipelines run: " << pipelinesCompleted << "\n";
        std::cout << "Output files created in current directory\n";
        std::cout << "=========================================\n";
        return 0;
    } else {
        LOG_ERROR("Pipeline execution failed");
        std::cerr << "\nERROR: Pipeline execution failed\n";
        return 1;
    }
}