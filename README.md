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
#include "cnumpy_allinone.c" // Or include its code block in your C project directly

int main() {
    double vals[] = {1.0, 2.0, 3.0, 4.0};
    CNumPyArray arr = create_array(vals, 4);
    CNumPyArray ones = array_ones(4);
    CNumPyArray sum_arr = add_array(&arr, &ones);
    print_array(&sum_arr, 2); // Output: [2.00, 3.00, 4.00, 5.00]
    printf("Mean: %f\n", mean_array(&sum_arr));
    free_array(&arr); free_array(&ones); free_array(&sum_arr);
    return 0;
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

---

## License 📜

MIT License

```
MIT License

Copyright (c) 2026  wangyifan349

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
```

