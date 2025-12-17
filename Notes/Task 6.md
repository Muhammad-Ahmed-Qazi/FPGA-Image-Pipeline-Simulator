# 6. Edge Detection Engine (Sobel / Laplacian)

## Task Description

**Theory:**

- Edge detection theory: gradients, Sobel kernels
- Combining convolution results (Gx, Gy)
- Clamping and magnitude calculation

**Deliverable:**

- `EdgeDetector` module applying Sobel filter
- Produces visible edge map from grayscale image

**Difficulty:** ★★★★☆ (challenging)



## Goal of Task 6

Simulate the **Edge Detection** stage of a hardware image-processing pipeline.
 You’ll implement a **Sobel edge detector** that identifies areas of strong intensity change (edges) in the grayscale image.

After this task, your pipeline will look like:

> **Load → Convert (Grayscale) → Smooth → Edge Detect → Save**



## Concepts Involved

### 1. Edges and Gradients

- An **edge** is where pixel intensity changes sharply, e.g. from light to dark.
- We detect this by finding the **gradient** (rate of change) in intensity across x and y directions.

### 2. Sobel Operators

Two 3x3 convolution kernels:
$$
G_x = \begin{bmatrix}-1 & 0 & +1 \\ -2 & 0 & +2 \\ -1 & 0 & + 1\end{bmatrix}
$$

$$
G_y = \begin{bmatrix}-1 & -2 & -1 \\ 0 & 0 & 0 \\ +1 & +2 & + 1\end{bmatrix}
$$

You convolve both with the image:

- $G_x$ detects **vertical edges**
- $G_y$ detects **horizontal edges**

Then, compute the **gradient magnitude**:
$$
\text{magnitude} = \sqrt{G_x^2 + G_y^2}
$$
To simplify, we’ll approximate this as:
$$
\text{magnitude} = |G_x| + |G_y|
$$
Finally, **clamp** this value to `[0, 255]` before writing it back to the image.