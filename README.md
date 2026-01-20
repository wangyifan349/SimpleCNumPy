# SimpleCNumPy 🚀

SimpleCNumPy aims to provide a clear, easy-to-use, and easy-to-learn numeric array library in pure C, inspired by Python’s NumPy. This project is perfect for students, teachers, and C programmers who want to introduce numerical computing concepts, practice array programming, or build simple data science utilities in C without heavyweight dependencies. All functions are written in a single C file with accurate English variable/function names and detailed comments, making it beginner-friendly, easily modifiable, and open source.

## Features ✨

- One-dimensional double array operations with clear struct-based API
- Array creation (zeros, ones, fill, range, linspace, copy)
- Elementwise math: add, subtract, multiply, divide, modulo, power, with both arrays and scalars
- Apply mathematical functions: sin, cos, tan, asin, acos, atan, exp, log, sqrt, abs, round, floor, ceil
- Array statistics and reduction: sum, mean, max, min, argmax, argmin, product, variance, standard deviation
- Simple linear algebra: dot product and L2 norm
- Utilities: clip, reverse, sort, unique, fill, comparison, any, all, print
- Single-file implementation: just compile and run!
- Thorough English comments and perfectly readable code

## Getting Started 🛠️

1. **Clone the repository:**  
   ```bash
   git clone https://github.com/wangyifan349/SimpleCNumPy.git
   cd SimpleCNumPy
   ```
2. **Build and Run:**  
   ```bash
   gcc cnumpy_allinone.c -o simplecnumpy_demo -lm
   ./simplecnumpy_demo
   ```
   Make sure you use `-lm` to link the math library!

## Example Usage 📚

Here’s what a typical use case looks like:

```c
#include "cnumpy_allinone.c"   // Make sure this file is in your project directory

int main() {
    // ===== Array Creation =====
    double nums[] = {1.0, 3.0, 5.0, 7.0};                       // Example raw values
    CNumPyArray arr = create_array(nums, 4);                     // Create array from values
    CNumPyArray arr_zeros = array_zeros(4);                      // Create array: [0,0,0,0]
    CNumPyArray arr_ones = array_ones(4);                        // Create array: [1,1,1,1]
    CNumPyArray arr_full = array_full(4, 9.9);                   // Create array: [9.9,9.9,9.9,9.9]
    CNumPyArray arr_range = array_range(2, 10, 2);               // Create array: [2,4,6,8]
    CNumPyArray arr_linspace = array_linspace(0, 1, 5);          // Create array: [0,0.25,0.5,0.75,1.0]
    CNumPyArray arr_copy = copy_array(&arr);                     // Copy arr

    // ===== Elementwise Operations (Array & Scalar) =====
    CNumPyArray arr_add = add_array(&arr, &arr_ones);            // [1,3,5,7]+[1,1,1,1]
    CNumPyArray arr_sub = subtract_array(&arr, &arr_ones);       // Subtraction
    CNumPyArray arr_mul = multiply_array(&arr, &arr_ones);       // Multiplication
    CNumPyArray arr_div = divide_array(&arr, &arr_ones);         // Division
    CNumPyArray arr_mod = modulo_array(&arr, &arr_ones);         // Modulo
    CNumPyArray arr_add_scalar = add_scalar(&arr, 10.0);         // Each element +10
    CNumPyArray arr_mul_scalar = multiply_scalar(&arr, 2.0);     // Each element *2

    // ===== Math Functions =====
    CNumPyArray arr_sin = sin_array(&arr);                       // Sine
    CNumPyArray arr_cos = cos_array(&arr);                       // Cosine
    CNumPyArray arr_exp = exp_array(&arr);                       // Exponential
    CNumPyArray arr_sqrt = sqrt_array(&arr);                     // Square root
    CNumPyArray arr_pow = pow_array(&arr, 3.0);                  // Each element to the power 3
    CNumPyArray arr_abs = absolute_array(&arr_sub);              // Absolute value

    // ===== Array Properties and Aggregation =====
    double sum_val = sum_array(&arr);                            // Sum of elements
    double mean_val = mean_array(&arr);                          // Mean value
    double min_val = min_array(&arr);                            // Minimum
    double max_val = max_array(&arr);                            // Maximum
    double var_val = variance_array(&arr);                       // Variance
    double std_val = std_array(&arr);                            // Standard deviation
    double prod_val = product_array(&arr);                       // Product of elements
    size_t idx_min = argmin_array(&arr);                         // Index of min
    size_t idx_max = argmax_array(&arr);                         // Index of max

    // ===== Linear Algebra =====
    double dot_val = dot_array(&arr, &arr_ones);                 // Dot product
    double l2_norm_val = l2_norm(&arr);                          // L2 norm

    // ===== Print & Utility Examples =====
    printf("arr = "); print_array(&arr, 2);                      // Print arr
    printf("arr_zeros = "); print_array(&arr_zeros, 1);          // Print zeros
    reverse_array(&arr_copy);                                    // Reverse arr_copy (in place)
    printf("Reversed arr_copy = "); print_array(&arr_copy,2);    // Print reversed
    fill_array(&arr_zeros, 8.8);                                 // Fill array with 8.8
    printf("arr_zeros filled = "); print_array(&arr_zeros,1);    // Print filled array

    int is_equal = equal_array(&arr, &arr_copy);                 // Compare arrays
    printf("arr == arr_copy? %d\n", is_equal);                   // 0 means not equal

    int has_any = any_array(&arr);                               // Any non-zero
    printf("Any nonzero element in arr? %d\n", has_any);         // 1 means true

    int has_all = all_array(&arr);                               // All non-zero
    printf("All nonzero elements in arr? %d\n", has_all);        // 1 means true

    CNumPyArray arr_clipped = clip_array(&arr, 2.5, 6.0);        // Clip each value between 2.5 and 6.0
    printf("Clipped arr = "); print_array(&arr_clipped,2);       // Print clipped

    sort_array(&arr_clipped);                                    // Sort in-place
    printf("Sorted arr_clipped = "); print_array(&arr_clipped,2);// Print sorted

    double vals2[] = {1,2,2,3,3,4};                             // Raw values with duplicates
    CNumPyArray tmp_arr = create_array(vals2, 6);                // Build array for unique
    CNumPyArray arr_unique = unique_array(&tmp_arr);             // Unique values (sorted)
    printf("Unique values in tmp_arr: "); print_array(&arr_unique,0);

    // ===== Print Aggregation Values =====
    printf("Sum: %.2f, Mean: %.2f, Min: %.2f (idx=%zu), Max: %.2f (idx=%zu)\n",
           sum_val, mean_val, min_val, idx_min, max_val, idx_max);
    printf("Variance: %.2f, Std: %.2f, Product: %.0f\n", var_val, std_val, prod_val);
    printf("Dot: %.2f, L2 norm: %.3f\n", dot_val, l2_norm_val);

    // ===== Free All Arrays =====
    free_array(&arr);
    free_array(&arr_zeros);
    free_array(&arr_ones);
    free_array(&arr_full);
    free_array(&arr_range);
    free_array(&arr_linspace);
    free_array(&arr_copy);
    free_array(&arr_add);
    free_array(&arr_sub);
    free_array(&arr_mul);
    free_array(&arr_div);
    free_array(&arr_mod);
    free_array(&arr_add_scalar);
    free_array(&arr_mul_scalar);
    free_array(&arr_sin);
    free_array(&arr_cos);
    free_array(&arr_exp);
    free_array(&arr_sqrt);
    free_array(&arr_pow);
    free_array(&arr_abs);
    free_array(&arr_clipped);
    free_array(&tmp_arr);
    free_array(&arr_unique);
    return 0; // Success
}
```

Explore the demo in the `main` function for more advanced examples!

## Why SimpleCNumPy? 🤔

- **Educational**: Perfect for teaching C array concepts and numeric programming.
- **Lightweight**: Zero dependencies, no makefile, portable and fast.
- **Readable code**: Standard English naming, detailed English/Chinese comments.
- **Expandable**: Start here if you want to experiment with your own C math libraries.

## Roadmap 📈

- Multi-dimensional arrays and matrix math (future)
- More math/statistics utilities
- Cross-platform enhancements

*Community contributions via PRs and issues are welcome!*
