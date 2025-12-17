# 3. Frame Reader & Writer Modules

## Task Description

**Theory:**

- File I/O in C++ (ifstream/ofstream)
- PPM/PGM image formats (plain-text RGB data)
- Mapping file data into pixel buffers
- Manual memory handling (load → process → free)

**Deliverable:**

- FrameReader that loads a small .ppm image into a pixel buffer
- FrameWriter that saves a processed buffer back to disk
- Verify correctness by round-trip (input → save → reopen → same)

**Difficulty:** ★★★☆☆ (moderate)



## Goal of Task 3

The goal of Task 3 is to **simulate how a hardware pipeline interacts with external memory** — by learning how to **load and store image data** from and to files in C++.

In hardware (e.g. FPGA-based systems), image data doesn’t just “exist” inside the chip. It’s fetched from a **frame buffer** in memory (e.g. DRAM), processed in the pipeline (filters, convolution, etc.), and then written back to memory for display or storage.

In your C++ simulation, the equivalent process is:

1. **Frame Reader** → loads pixels from an image file (like from external memory).
2. **Processing pipeline** → operates on that data (coming later).
3. **Frame Writer** → writes processed pixels back into a file (like saving to memory or sending to a display).



## Concepts Involved

### 1. File I/O in C++

- Reading and writing files using `ifstream` and `ofstream`.
- Conceptually similar to how a hardware block reads from memory or a camera sensor.

### 2. Image File Formats (Simplified)

- For simplicity, use PPM (Portable PixMap) format:
  - Plain text format storing RGB pixel values.
  - Easy to read/write without libraries.

### 3. Mapping File Data to Buffers

- Read pixel values from the file and store them in a dynamically allocated buffer.
- Write back from buffer to file using the same order.

### 4. Memory Management

- Still use `new/delete` as in Task 2.
- FrameReader allocates memory; FrameWriter does not.
- Responsibility: avoid leaks.

### 5. Data Flow Thinking

- Input --> buffer --> processing modules --> output.