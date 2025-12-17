# 2. Image Representation & Memory Fundamentals

## Task Description

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



## Goal of Task 2

To understand **how digital images are stored in memory** and to **implement that concept** using a simple C++ program that creates, allocates, and frees an image buffer dynamically.



## Concepts Involved

### 1. Pixel Representation

- A pixel (picture element) is the smallest unit of an image.
- Each pixel typically stores 3 colour components:
  - R (Red), G (Green), B (Blue)
- Each component is commonly 8 bits, so one pixel = 24 bits (3 bytes).

### 2. 2D Image in Linear Memory

- Although an image is 2D (`width x height`), memory in C++ is linear (1D).

- So, to store an image, we flatten it:

  `index = y * width + x;`

- This is critical because FPGAs process pixel data linearly - streaming one pixel per clock cycle.

### 3. Pointers and Dynamic Memory Allocation

- We'll use dynamic memory because we may not know image size at compile-time.
- This allocates enough memory for all pixels.
- Remember to free memory after use.

### 4. Structs for Lightweight Data Storage

- A `struct` groups related variables - perfect for defining a pixel.
- Each pixel is compact (3 bytes) and easy to manage.

