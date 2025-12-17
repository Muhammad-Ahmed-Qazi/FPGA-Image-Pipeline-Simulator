# 4. Colour Converter Module (RGB → Grayscale)

## Task Description

**Theory:**

- Per-pixel operations and iteration
- Grayscale conversion formula:
  `Y = 0.299R + 0.587G + 0.114B`
- Data-flow thinking: input buffer → output buffer

**Deliverable:**

- `ColorConverter` struct/class with `convert()` function
- Produces grayscale version of input image (verify visually or via pixel values)

**Difficulty:** ★★★☆☆ (moderate)



## Goal of Task 4

To **convert a colour image to grayscale** — just like what happens inside a hardware image-processing block on an FPGA before filtering or edge detection.
 In this task, you’ll:

- Read an RGB image into memory (already done by FrameReader).
- Process each pixel to compute its grayscale intensity.
- Write out the result to a new PPM image (e.g. `gray_output.ppm`).



## Concepts Involved

### 1. What is Grayscale Conversion?

- A grayscale pixel only represents **luminance** (brightness).

- We compute it as a weighted average of R, G, and B values.

  `Y = 0.299R + 0.587G + 0.114B`

- This gives a **single-channel intensity**, which can be stored as `r=g=b=Y` to keep the same struct layout.

### 2. What's happening in FPGA-like modelling terms?

In real hardware, this would be:

- Each pixel enters the pipeline stage.
- Three multipliers and two adders compute the weighted sum.
- Output is stored in the next buffer/register.

In the C++ simulation:

- You'll loop through the pixel buffer.
- Compute grayscale intensity.
- Store it back to the same buffer or a new one.

This mimics **streaming data through a hardware pipeline** pixel by pixel.

### 3. Memory Access

Each pixel lives in linear memory, so they can be accessed using a simple for loop.

This simulates how a pipeline of processing cores would 'process' an image.

