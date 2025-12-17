#ifndef BUFFER_H
#define BUFFER_H

#include "config.h"
#include "pixel.h"
#include <memory>
#include <cstring>
#include <iostream>

namespace hardware {
    namespace memory {
        
        // Simulates hardware frame buffer with alignment
        class FrameBuffer {
        private:
            pixel* data;
            int width;
            int height;
            size_t capacity;
            bool ownsMemory;
            
        public:
            // Constructor with allocation
            FrameBuffer(int w, int h) 
                : width(w), height(h), 
                  capacity(w * h * sizeof(pixel)),
                  ownsMemory(true) {
                
                #ifdef HW_SIMULATION
                    // Simulate aligned memory allocation for hardware
                    data = static_cast<pixel*>(aligned_alloc(FRAME_BUFFER_ALIGNMENT, capacity));
                #else
                    data = new pixel[width * height];
                #endif
                
                LOG_MEMORY_ALLOC(capacity, data);
                LOG_INFO("FrameBuffer created: " << width << "x" << height);
            }
            
            // Constructor wrapping existing memory
            FrameBuffer(pixel* existingData, int w, int h, bool takeOwnership = false)
                : data(existingData), width(w), height(h),
                  capacity(w * h * sizeof(pixel)),
                  ownsMemory(takeOwnership) {
                LOG_INFO("FrameBuffer wrapped existing memory");
            }
            
            ~FrameBuffer() {
                if (ownsMemory && data) {
                    #ifdef HW_SIMULATION
                        free(data);
                    #else
                        delete[] data;
                    #endif
                    LOG_MEMORY_FREE(data);
                }
                data = nullptr;
            }
            
            // Prevent copying
            FrameBuffer(const FrameBuffer&) = delete;
            FrameBuffer& operator=(const FrameBuffer&) = delete;
            
            // Allow moving
            FrameBuffer(FrameBuffer&& other) noexcept
                : data(other.data), width(other.width), height(other.height),
                  capacity(other.capacity), ownsMemory(other.ownsMemory) {
                other.data = nullptr;
                other.ownsMemory = false;
            }
            
            FrameBuffer& operator=(FrameBuffer&& other) noexcept {
                if (this != &other) {
                    if (ownsMemory && data) {
                        #ifdef HW_SIMULATION
                            free(data);
                        #else
                            delete[] data;
                        #endif
                    }
                    
                    data = other.data;
                    width = other.width;
                    height = other.height;
                    capacity = other.capacity;
                    ownsMemory = other.ownsMemory;
                    
                    other.data = nullptr;
                    other.ownsMemory = false;
                }
                return *this;
            }
            
            // Accessors
            pixel* getData() { return data; }
            const pixel* getData() const { return data; }
            int getWidth() const { return width; }
            int getHeight() const { return height; }
            size_t getSize() const { return width * height; }
            size_t getCapacity() const { return capacity; }
            
            // Operations
            void clear() {
                if (data) {
                    memset(data, 0, capacity);
                    LOG_VERBOSE("FrameBuffer cleared");
                }
            }
            
            void copyFrom(const pixel* source) {
                if (data && source) {
                    memcpy(data, source, width * height * sizeof(pixel));
                    LOG_VERBOSE("FrameBuffer copied from source");
                }
            }
            
            void copyTo(pixel* dest) const {
                if (data && dest) {
                    memcpy(dest, data, width * height * sizeof(pixel));
                    LOG_VERBOSE("FrameBuffer copied to destination");
                }
            }
            
            void dumpInfo() const {
                LOG_INFO("FrameBuffer Info:");
                LOG_INFO("  Dimensions: " << width << "x" << height);
                LOG_INFO("  Size: " << (width * height) << " pixels");
                LOG_INFO("  Capacity: " << capacity << " bytes");
                LOG_INFO("  Memory owned: " << (ownsMemory ? "yes" : "no"));
                LOG_INFO("  Data pointer: " << static_cast<void*>(data));
                
                #ifdef HW_SIMULATION
                    LOG_HARDWARE("  Hardware aligned: " << 
                                 ((reinterpret_cast<uintptr_t>(data) % FRAME_BUFFER_ALIGNMENT == 0) 
                                  ? "yes" : "no"));
                #endif
            }
        };
        
        // Simulates hardware FIFO (First-In-First-Out buffer)
        template<typename T, int CAPACITY>
        class FIFO {
        private:
            T buffer[CAPACITY];
            int head;
            int tail;
            int count;
            
            #ifdef HW_SIMULATION
                // Hardware simulation registers
                int overflowCount;
                int underflowCount;
            #endif
            
        public:
            FIFO() : head(0), tail(0), count(0) {
                #ifdef HW_SIMULATION
                    overflowCount = 0;
                    underflowCount = 0;
                #endif
                LOG_VERBOSE("FIFO created with capacity " << CAPACITY);
            }
            
            bool push(const T& item) {
                if (count >= CAPACITY) {
                    LOG_WARNING("FIFO overflow");
                    #ifdef HW_SIMULATION
                        overflowCount++;
                        DUMP_REGISTER("FIFO_OVERFLOW", overflowCount);
                    #endif
                    return false;
                }
                
                buffer[tail] = item;
                tail = (tail + 1) % CAPACITY;
                count++;
                
                LOG_VERBOSE("FIFO push: count=" << count);
                #ifdef HW_SIMULATION
                    DUMP_REGISTER("FIFO_COUNT", count);
                #endif
                
                return true;
            }
            
            bool pop(T& item) {
                if (count <= 0) {
                    LOG_WARNING("FIFO underflow");
                    #ifdef HW_SIMULATION
                        underflowCount++;
                        DUMP_REGISTER("FIFO_UNDERFLOW", underflowCount);
                    #endif
                    return false;
                }
                
                item = buffer[head];
                head = (head + 1) % CAPACITY;
                count--;
                
                LOG_VERBOSE("FIFO pop: count=" << count);
                return true;
            }
            
            bool isEmpty() const { return count == 0; }
            bool isFull() const { return count == CAPACITY; }
            int size() const { return count; }
            int getCapacity() const { return CAPACITY; }
            
            #ifdef HW_SIMULATION
                int getOverflowCount() const { return overflowCount; }
                int getUnderflowCount() const { return underflowCount; }
                
                void resetStats() {
                    overflowCount = 0;
                    underflowCount = 0;
                }
            #endif
            
            void clear() {
                head = 0;
                tail = 0;
                count = 0;
                LOG_VERBOSE("FIFO cleared");
            }
        };
        
        // Common FIFO types for pipeline
        using PixelFIFO64 = FIFO<pixel, 64>;
        using PixelFIFO128 = FIFO<pixel, 128>;
        using PixelFIFO256 = FIFO<pixel, 256>;
        
    } // namespace memory
} // namespace hardware

#endif // BUFFER_H