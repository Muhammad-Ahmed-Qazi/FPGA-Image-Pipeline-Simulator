#ifndef CONFIG_H
#define CONFIG_H

// Add this near other configuration macros
#ifndef HAS_CONVOLUTION
#define HAS_CONVOLUTION 1  // Enable convolution engine
#endif

// ============================================================================
// FPGA Image Processing Pipeline Configuration
// ============================================================================

// Namespace for hardware simulation
namespace hardware {
    namespace pipeline {
        // Pipeline configuration constants
        constexpr int DEFAULT_WIDTH = 640;
        constexpr int DEFAULT_HEIGHT = 480;
        constexpr int MAX_PIPELINE_STAGES = 10;
        
        // Processing modes
        enum class ProcessingMode {
            FLOATING_POINT,
            FIXED_POINT,
            HARDWARE_SIM
        };
    }
    
    namespace memory {
        // Memory configuration
        constexpr int FRAME_BUFFER_ALIGNMENT = 64;  // Cache line alignment
        constexpr int MAX_FRAME_BUFFERS = 4;
    }
    
    namespace filters {
        // Filter kernel sizes
        template<int N>
        struct KernelSize {
            static constexpr int value = N;
            static constexpr int radius = N / 2;
        };
        
        // Common kernel sizes
        using Kernel3x3 = KernelSize<3>;
        using Kernel5x5 = KernelSize<5>;
        using Kernel7x7 = KernelSize<7>;
    }
}

// ============================================================================
// Compile-time Configuration Macros
// ============================================================================

// Default image dimensions (can be overridden)
#ifndef IMAGE_WIDTH
    #define IMAGE_WIDTH  640
#endif

#ifndef IMAGE_HEIGHT
    #define IMAGE_HEIGHT 480
#endif

// Hardware simulation parameters
#ifdef HW_SIMULATION
    #define CLOCK_CYCLES_PER_PIXEL 4
    #define PIPELINE_DEPTH 5
    #define MEMORY_BANDWIDTH 100  // MB/s
    #define BUS_WIDTH 32          // bits
    #define CLOCK_FREQUENCY 100   // MHz
    
    // Simulate hardware registers
    #define REGISTER_WIDTH 32
    #define NUM_HW_REGISTERS 16
#endif

// ============================================================================
// Debug and Logging System
// ============================================================================

#ifdef DEBUG
    #include <iostream>
    #include <iomanip>
    #include <chrono>
    
    // Debug levels
    enum class DebugLevel {
        NONE = 0,
        ERROR = 1,
        WARNING = 2,
        INFO = 3,
        VERBOSE = 4,
        HARDWARE = 5
    };
    
    #ifndef CURRENT_DEBUG_LEVEL
        #define CURRENT_DEBUG_LEVEL DebugLevel::INFO
    #endif
    
    // Timestamp for debug messages
    inline std::string debugTimestamp() {
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;
        
        std::stringstream ss;
        ss << std::put_time(std::localtime(&time), "%H:%M:%S");
        ss << '.' << std::setfill('0') << std::setw(3) << ms.count();
        return ss.str();
    }
    
    // Debug logging macros
    #define LOG_ERROR(msg) \
        if (CURRENT_DEBUG_LEVEL >= DebugLevel::ERROR) \
            std::cerr << "[" << debugTimestamp() << "] [ERROR] " \
                      << __FILE__ << ":" << __LINE__ << " " << msg << std::endl
    
    #define LOG_WARNING(msg) \
        if (CURRENT_DEBUG_LEVEL >= DebugLevel::WARNING) \
            std::cout << "[" << debugTimestamp() << "] [WARNING] " << msg << std::endl
    
    #define LOG_INFO(msg) \
        if (CURRENT_DEBUG_LEVEL >= DebugLevel::INFO) \
            std::cout << "[" << debugTimestamp() << "] [INFO] " << msg << std::endl
    
    #define LOG_VERBOSE(msg) \
        if (CURRENT_DEBUG_LEVEL >= DebugLevel::VERBOSE) \
            std::cout << "[" << debugTimestamp() << "] [VERBOSE] " << msg << std::endl
    
    #define LOG_HARDWARE(msg) \
        if (CURRENT_DEBUG_LEVEL >= DebugLevel::HARDWARE) \
            std::cout << "[" << debugTimestamp() << "] [HW] " << msg << std::endl
    
    // Hardware register simulation
    #define DUMP_REGISTER(name, value) \
        LOG_HARDWARE("REG " << name << " = 0x" << std::hex << value << std::dec)
    
    #define LOG_STAGE(stage, msg) \
        LOG_INFO("[" << stage << "] " << msg)
    
    // Pipeline stage tracking
    #define ENTER_STAGE(stage_name) \
        LOG_VERBOSE("Entering stage: " << stage_name)
    
    #define EXIT_STAGE(stage_name) \
        LOG_VERBOSE("Exiting stage: " << stage_name)
    
    // Memory tracking
    #define LOG_MEMORY_ALLOC(size, ptr) \
        LOG_VERBOSE("Allocated " << size << " bytes at " << ptr)
    
    #define LOG_MEMORY_FREE(ptr) \
        LOG_VERBOSE("Freed memory at " << ptr)
    
#else
    // No-op macros when DEBUG is not defined
    #define LOG_ERROR(msg)
    #define LOG_WARNING(msg)
    #define LOG_INFO(msg)
    #define LOG_VERBOSE(msg)
    #define LOG_HARDWARE(msg)
    #define DUMP_REGISTER(name, value)
    #define LOG_STAGE(stage, msg)
    #define ENTER_STAGE(stage_name)
    #define EXIT_STAGE(stage_name)
    #define LOG_MEMORY_ALLOC(size, ptr)
    #define LOG_MEMORY_FREE(ptr)
#endif

// ============================================================================
// Function Pointer Types for Dynamic Filter Registration
// ============================================================================

// Filter creation function pointer type
typedef class BaseFilter* (*FilterCreator)();

// Filter application function pointer type
typedef void (*FilterApplyFunc)(void* filter, 
                                struct pixel* input, 
                                struct pixel* output, 
                                int width, int height);

// ============================================================================
// Template Helper for Kernel Operations
// ============================================================================

// Template for compile-time kernel operations
template<typename T, int KERNEL_SIZE>
class KernelProcessor {
public:
    static constexpr int RADIUS = KERNEL_SIZE / 2;
    
    // Apply kernel to a pixel
    template<typename Func>
    static T processPixel(int x, int y, int width, int height, 
                         const T* input, const float kernel[KERNEL_SIZE][KERNEL_SIZE],
                         Func processFunc) {
        T result = T();
        for (int ky = -RADIUS; ky <= RADIUS; ky++) {
            for (int kx = -RADIUS; kx <= RADIUS; kx++) {
                int idx = (y + ky) * width + (x + kx);
                float weight = kernel[ky + RADIUS][kx + RADIUS];
                result = processFunc(result, input[idx], weight);
            }
        }
        return result;
    }
};

// Specialization for common kernel sizes
using KernelProcessor3x3 = KernelProcessor<float, 3>;
using KernelProcessor5x5 = KernelProcessor<float, 5>;

#endif // CONFIG_H