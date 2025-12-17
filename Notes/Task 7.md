# 7. Pipeline integration

## Task Description

**Theory:**

- Software simulation of data flow: sequential vs parallel
- Passing buffers between modules safely
- Lifetime of dynamically allocated data

**Deliverable:**

- `Pipeline` class controlling all stages:
  `load -> convert -> smooth -> edge -> save`

**Difficulty:** ★★★☆☆ (moderate)



## Goal of Task 7

The goal is to **combine all previously developed modules** (Frame Reader, Color Converter, Smoothing Filter, and Edge Detector) into a single, coherent **`Pipeline` class** that simulates a real **hardware data flow** — just like how an FPGA passes frames between functional blocks.

You’ll have:

```
FrameReader → ColorConverter → SmoothingFilter → EdgeDetector → FrameWriter
```

Each stage takes an input buffer, processes it, and passes it to the next.



## Concepts Involved

### 1. Software simulation of data flow

- In real FPGA systems, data flows through hardware *registers* and *FIFOs* between processing blocks.
- Here, we simulate that behaviour by sequentially calling C++ functions and passing *pointers to buffers* between stages.
- Though sequential in software, it mimics a *pipelined* hardware design.

### 2. Buffer ownership & lifetime

- Each processing stage might allocate a new output buffer.
- You must **free** buffers you no longer need to avoid memory leaks.
- Example:
  - Reader allocates --> Grayscale converter reuses same buffer
  - Smoothing creates new --> Edge detection creates another new --> Writer saves and frees all.

### 3. Encapsulation & abstraction

- The `Pipeline` class hides all internal details.
- The `main.cpp` will only create a pipeline and call one function like.

