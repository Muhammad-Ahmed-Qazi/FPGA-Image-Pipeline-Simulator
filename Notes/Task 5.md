# 5. Smoothing Filter (3x3 Average)

## Task Description

**Theory:**

- Convolution basics and kernel matrices
- Handling image borders
- Pointer arithmetic for accessing neighbouring pixels

**Deliverable:**

- `SmoothingFilter` that applies a 3×3 average kernel
- Demonstration on grayscale image
- Prints before/after pixel samples for verification

**Difficulty:** ★★★★☆ (challenging)

### 

## Goal of Task 5

To implement a **3×3 average filter** that *smooths* (blurs) an image by averaging each pixel with its eight neighbours.

In this stage, you’ll:

- Read the grayscale image (from your previous task).
- Create a new image buffer for the filtered output.
- For every pixel, compute the average of its 3×3 neighbourhood.
- Write the result to an output file (`smooth_output.ppm`).

This corresponds to the **Filter stage** in your image pipeline:

```
Load → Convert → Filter → Edge Detect → Save
```

In FPGA terms, this mimics a **convolution engine** that:

- Reads pixels from a streaming buffer.
- Multiplies them with filter coefficients.
- Accumulates the sum.
- Writes the output pixel.



## Concepts Involved

### 1. Convolution Basics

- Convolution is a mathematical operation that combines an image with a *kernel* (small matrix) to modify the image.

- For a 3x3 average filter, the kernel looks like this:
  $$
  K = \frac{1}{9}{\begin{bmatrix} 1 & 1 & 1 \\ 1 & 1 & 1 \\ 1 & 1 & 1\end{bmatrix}}
  $$

- Each output pixel = average of itself and its 8 neighbours:
  $$
  I'(x, y) = \frac{1}{9}\sum^1_{i=-1}\sum^1_{j=-1} I(x + i, y + j)
  $$

- This operation reduces noise and smooths the image.

### 2. Kernel Application (Neighbourhood Access)

For each pixel, you look at its surrounding pixels.
 In memory (which is linear), that means you must know how to move:

- Left = `-1`
- Right = `+1`
- Up = `-width`
- Down = `+width`

If your pixel buffer is 1D (`pixel* frame = new pixel[w*h];`),
 the pixel at (x, y) is located at:
$$
index = y * width + x
$$
So to get its top-left neighbour, you do:
$$
frame[(y-1)*width + (x-1)]
$$

### 3. Handling Borders

Common strategies for when some neighbours of an image don't exist:

- Ignore borders (process from 1 to width - 2, 1 to height - 2)
- Clamp (use nearest pixel)
- Wrap around (not common for filtering)

For simplicity, we'll only use the inner pixels.

### 4. Pointer Arithmetic & Memory Management

As this operation reads and writes simultaneously, we will:

- Keep one buffer for input (original frame)
- One buffer for output (filtered image)
- Then copy or save the result after processing

This models **stream buffering or dual-port memory** on FPGAs, where input and output frames are separate memory regions.

### 5. Hardware Analogy

In hardware:

- A 3x3 filter is implemented using **line buffers** and **shift registers** to store 3 rows of pixels.
- As new pixels stream in, the 3x3 window slides across the image.
- Multipliers and adders compute the weighted sum in parallel.

My C++ code with compute this sequentially.

