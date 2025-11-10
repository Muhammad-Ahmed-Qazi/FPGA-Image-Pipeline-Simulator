# FPGA-Based Image Processing Pipeline Simulator Using C++

**Xcelerium Training Programme - Module 1 Project**

[TOC]



## ❓ Why I am Building This?

To learn *how hardware handles image data in real-word FPGA systems*. It is used to power embedded vision systems like:

- self-driving cameras
- drones
- industrial inspection systems

This project will help me think like a hardware designer:

- Break big operations (like filtering) into smaller hardware modules.
- Connect those modules using memory buffers
- Use registers and streams to pass data - not high-level libraries.

## 🤔 How is it FPGA-Based?

| Concept          | Software (like Python/OpenCV)              | Hardware (FPGA)                                              |
| ---------------- | ------------------------------------------ | ------------------------------------------------------------ |
| Processing Style | Sequential - one pixel at a time, in loops | Parallel - multiple pixels processed simultaneously in pipeline stages |
| Data storage     | Whole image stored in RAM                  | Streams of pixels passed through registers and FIFOs         |
| Execution        | Controlled by CPU instructions             | Controlled by hardware logic (gates, registers, FSMs)        |
| Code             | Functions calling each other               | Hardware blocks connected in sequence                        |
| Debugging        | Print/log statements                       | Signal probes or debug flags                                 |

This simulator is a **software imitation** of this hardware structure. So, generally, each part of my C++ code will represent one FPGA hardware block.

## 🗺️ Conceptual Map of the Whole Project

### 1. Image Representation Concepts

How we *store and interpret* image data in memory

- Pixel
- Frame Buffer

### 2. Memory Management Concepts

How to handle image data *like hardware in memory*

- Pointers
- Dynamic Memory

### 3. Modular Design and Data-flow

The pipeline is a **chain of modules**, each performing one operation and passing data forward (like FPGA logic blocks).

- Modules as Structs/Classes
- Pipeline

### 4. C++ Programming Concepts Used

How C++ knowledge connects directly with hardware thinking

- **Structs & Classes**: Represent hardware modules or data types
- **Polymorphism**: Different filters share a base 'Filter' interface
- **Typedef / using**: Define custom data types
- **Preprocessor (`#ifdef`)**: Compile different hardware modes
- **Multiple source files**: Separate each module into its own file, just like separate hardware blocks

### 5. Digital Signal / Image Processing Concepts

- **Colour conversion**: RGB -> Grayscale
- **Smoothing Filter**: 3x3 averaging kernel
- **Convolution Engine**: Matrix operation
- **Edge Detection**: Sobel/Laplacian

### 6. Hardware Modeling Mindset

Thinking like hardware while coding in C++

- **Registers**: Small temporary storage (simulate with variables)
- **FIFOs / Buffers**: Temporary pixel storage between modules
- **Clock cycles / pipeline latency**: Represented by sequential data passing
- **Fixed-point arithmetic**: Replaces floating-point for faster, FPGA-style math
- **Debug signals**: Use `#ifdef DEBUG` to print internal values (like hardware debug ports)

### 7. Software Architecture Concepts

To keep the project structured like a real system.

- **Header files (`.h`)**: Declare data types and module interfaces
- **Source files (`.cpp`)**: Define the actual logic
- **Makefile**: Automates compilation and linking
- **Modular compilation**: Allows rebuilding only changed modules
- **Main function**: Acts as the "system controller" that connects all modules

### 8. Optional Advanced Concepts

- Fixed vs Floating point simulation (`#ifdef USE_FIXED_POINT`)
- Parallelism simulation (threads or pipelining)
- Performance measurement (simulated latency)
- Hardware register logging (`#ifdef DEBUG` macros)



## 📋 Task Breakdown

### 1. Project Orientation & Setup 

**Theory:**

- What an FPGA-style image pipeline is and why we simulate it
- Overview of project structure (modules, headers, Makefile)
- Refresh on C++ compilation and linking

**Deliverable:**

- Folder tree ready with empty `.cpp` and `.h` files
- Skeleton `Makefile` that builds a dummy `main.cpp`

**Difficulty:** ★☆☆☆☆ (very easy)

### 2. Image Representation & Memory Fundamentals

**Theory:**

- What a pixel is (RGB values, 8-bit channels)
- How a 2-D image maps to linear memory
- Basics of pointers, arrays, and dynamic memory allocation (`new`/`delete`)
- Structs vs classes for lightweight data storage

**Deliverable:**

- `pixel` struct defined in `pixel.h`
- Dynamically allocated image buffer (e.g. `pixel* frame = new pixel[w*h];`)
- Simple test in `main.cpp` that allocates and frees memory

**Difficulty:** ★★☆☆☆ (easy)

### 3. Frame Reader & Writer Modules

**Theory:**

- File I/O in C++ (`ifstream`/`ofstream`)
- PPM/PGM image formats (plain-text RGB data)
- Mapping file data into pixel buffers
- Manual memory handling (load → process → free)

**Deliverable:**

- `FrameReader` that loads a small `.ppm` image into a pixel buffer
- `FrameWriter` that saves a processed buffer back to disk
- Verify correctness by round-trip (input → save → reopen → same)

**Difficulty:** ★★★☆☆ (moderate)

### 4. Colour Converter Module (RGB → Grayscale)

**Theory:**

- Per-pixel operations and iteration
- Grayscale conversion formula:
   `Y = 0.299R + 0.587G + 0.114B`
- Data-flow thinking: input buffer → output buffer

**Deliverable:**

- `ColorConverter` struct/class with `convert()` function
- Produces grayscale version of input image (verify visually or via pixel values)

**Difficulty:** ★★★☆☆ (moderate)

### 5. Smoothing Filter (3x3 Average)

**Theory:**

- Convolution basics and kernel matrices
- Handling image borders
- Pointer arithmetic for accessing neighbouring pixels

**Deliverable:**

- `SmoothingFilter` that applies a 3×3 average kernel
- Demonstration on grayscale image
- Prints before/after pixel samples for verification

**Difficulty:** ★★★★☆ (challenging)

### 6. Edge Detection Engine (Sobel / Laplacian)

**Theory:**

- Edge detection theory: gradients, Sobel kernels
- Combining convolution results (Gx, Gy)
- Clamping and magnitude calculation

**Deliverable:**

- `EdgeDetector` module applying Sobel filter
- Produces visible edge map from grayscale image

**Difficulty:** ★★★★☆ (challenging)

### 7. Pipeline integration

**Theory:**

- Software simulation of data flow: sequential vs parallel
- Passing buffers between modules safely
- Lifetime of dynamically allocated data

**Deliverable:**

- `Pipeline` class controlling all stages:
  `load -> convert -> smooth -> edge -> save`

**Difficulty:** ★★★☆☆ (moderate)

### 8. Polymorphism & OOP Refactor

**Theory:**

- Base class and virtual functions
- Runtime polymorphism for filters (`apply()` interface)
- Inheritance and dynamic dispatch

**Deliverable:**

- `BaseFilter` abstract class
- Derived classes: `SmoothingFilter`, `EdgeFilter`, etc.
- Unified interface so pipeline can swap filters dynamically

**Difficulty:** ★★★★☆ (challenging, but short)

### 9. Hardware Mindset Enhancements

**Theory:**

- Fixed-point vs floating-point arithmetic
- Preprocessor directives (`#ifdef USE_FIXED_POINT`)
- Debug macros (`#ifdef DEBUG`)
- Simulating registers and internal logs

**Deliverable:**

- Compile-time switch for fixed-point simulation
- Debug prints showing intermediate register values
- Verified memory cleanup (`delete[]`)

**Difficulty:** ★★★☆☆ (moderate)

### 10. Final Testing & Documentation

**Theory:**

- Testing methodology for image systems
- Measuring runtime and verifying correctness
- Writing short project report / README

**Deliverable:**

- Working executable `pipeline_sim`
- README summarising modules, theory, and results
- Optional before-after image examples

**Difficulty:** ★★☆☆☆ (easy)