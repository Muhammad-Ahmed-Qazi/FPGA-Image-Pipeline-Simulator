# 8. Polymorphism & OOP Refactor

## Task Description

**Theory:**

- Base class and virtual functions
- Runtime polymorphism for filters (`apply()` interface)
- Inheritance and dynamic dispatch

**Deliverable:**

- `BaseFilter` abstract class
- Derived classes: `SmoothingFilter`, `EdgeFilter`, etc.
- Unified interface so pipeline can swap filters dynamically

**Difficulty:** ★★★★☆ (challenging, but short)



## Goal of Task 8

Transform your filter system into an **object-oriented, polymorphic filter engine**, so that:

- All filters share a **common interface**
- Pipeline can apply ANY filter without knowing its type
- You can add new filters later **without changing existing code**

This lets you write:

```c++
BaseFilter* f = new SmoothingFilter();
f->apply(input, output, width, height);
```

or swap it easily:

```C++
BaseFilter* f = new EdgeFilter();
```



## Concepts Involved

### 1. Abstract Base Class

We define a parent class:

```C++
class BaseFilter {
public:
	virtual void apply(pixel* in, pixel* out, int w, int h) = 0;
	virtual ~BaseFilter() = default;
}
```

- `= 0` --> makes `apply()` **pure virtual**, so class is abstract.

### 2. Inheritance

Each filter becomes a subclass:

```C++
class SmoothingFilter : public BaseFilter { ... };
class EdgeFilter : public BaseFilter { ...};
```

### 3. Runtime Polymorphism

We store a pointer to **BaseFilter** but object type can change at runtime.

So pipeline can do:

```C++
BaseFilter* f = new SmoothingFilter();
f->apply(...);
```

and you never need to write special cases.

### 4. Dynamic Dispatch

The correct `apply()` (smooth/edge) is decided at **runtime**, NOT compile time.