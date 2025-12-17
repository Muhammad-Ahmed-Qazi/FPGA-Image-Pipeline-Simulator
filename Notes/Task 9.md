# 9. Hardware Mindset Enhancements

## Task Description

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



## Goal of Task 9

Task 9 pushes your software simulation toward a *hardware-style* mindset:

- Treat your processing as if it were running on a small hardware block.
- Add **fixed-point (integer) arithmetic mode** to mimic hardware DSP units.
- Add **debug logging** that behaves like "register dumps" inside hardware.
- Add **compile-time switches** (not runtime if/else) so your binary behaves differently depending on what flags you build with.

This task makes your code feel like something that could eventually be **synthesised onto hardware**, similar to FPGA/ASIC design flows.



## Concepts Involved

### 1. Fixed-Point Arithmetic

- Hardware rarely uses floating-point (too expensive)
- Instead, it often uses integers representing scaled values
- Example format: `Q8, 8` --> 8 bits integer + 8 bits fractional.

We simulate this using:

```
typedef int Fixed; // using 32-bit signed integer
#define FP_SCALE (256)
```

### 2. Compile-Time Switches with `#ifdef`

You will introduce flags:

- `USE_FIXED_POINT`
  - Enables fixed-point mode instead of float/rgb arithmetic
- `DEBUG`
  - Enables debug print statements showing internal registers

These are enabled in the Makefile, not in C++ code.

### 3. Debug Logs (Simulating Hardware Registers)

When `DEBUG` is enabled, every stage prints important values:

- Pixel input register
- Pixel output register
- Intermediate filtered values

This mimics RTL signal prints or 'probe' outputs.

### 4. Memory Integrity

- Task also verifies that all dynamically allocated memory is properly released (`delete[]`).

We have already done most of this, so no major change needed.