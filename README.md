# Register Usage and Pointer Elimination
## Overview
This project demonstrates the performance impact of pointer dereferencing versus optimized direct indexing in C++ matrix operations. It compares two functions: one using heavy pointer dereferencing (`pointer_sum()`) and another using temporary pointers with direct indexing (`pointer_elim_sum()`). The program measures runtime differences and explores the effect of the RESTRICT macro for compiler optimizations.

## Build & Run
```bash
# Clone the repository
git clone https://github.com/AniDashyan/register_and_pointer_elimination
cd register_and_pointer_elimination


# Build the project
cmake -S . -DUSE_RESTRICT=ON -B build
cmake --build . --config Release


# Run the executable:
./main

# Optionally, enable the RESTRICT macro at runtime by passing the --restrict flag:
./main --restrict
```


## Example Output
```bash
RESTRICT is enabled
Using RESTRICT macro:  __restrict
Pointer-based Sum: 49531227, Time: 234 us
Optimized Sum: 49531227, Time: 204 us
```

## How Does It Work?
The program creates a 1000x1000 matrix filled with random integers (0-99) and computes the sum of all elements using two methods:

**Pointer-based (`pointer_sum()`)**: Uses double pointer dereferencing `matrix[i][j]` in nested loops, which can lead to redundant memory accesses and cache inefficiencies.
**Optimized (`pointer_elim_sum()`)**: Uses a temporary pointer `row = matrix[i]` to reduce dereferencing to direct indexing `row[j]`, allowing the compiler to optimize memory access and utilize registers more effectively.

**Potential Bottlenecks**: Excessive pointer dereferencing can cause cache misses and increased memory access latency, as the CPU must resolve multiple memory addresses per iteration. This is mitigated in the optimized version by reducing pointer arithmetic.
**RESTRICT Macro**: The `RESTRICT` keyword `__restrict` informs the compiler that pointers do not alias, enabling aggressive optimizations like loop unrolling and register allocation. The program allows enabling RESTRICT via a CMake flag `-DUSE_RESTRICT=ON` and a runtime flag `--restrict`, demonstrating its impact on performance.
