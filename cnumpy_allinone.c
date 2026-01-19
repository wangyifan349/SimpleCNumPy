/**
 * ===========================================================================
 *                    CNumPy: A Minimal Numeric Array Library in C
 * ===========================================================================
 *
 * Description:
 *   This all-in-one C code implements a minimalistic, easy-to-read "numeric array" library
 *   for one-dimensional double arrays, designed as a learning/demo open source project
 *   inspired by Python's NumPy. It covers:
 *     - Array creation (with zeros, ones, sequence, full, copy)
 *     - Element-wise operations (add, subtract, multiply, divide, modulo, power, with arrays or scalars)
 *     - Aggregation/statistics (sum, mean, min, max, argmin, argmax, prod, variance, stddev)
 *     - Element-wise math functions (sin, cos, exp, log, sqrt, abs, round, floor, ceil, tan, asin, acos, atan)
 *     - Simple vector linear algebra (dot product, L2 norm)
 *     - Array utilities (print, reverse, fill, compare, unique, sort, clip, any, all)
 *     - Range and linspace
 *
 *   All variable and function names use clear, standard English.
 *   The code is written for clarity: no macro/function pointer dark magic, no unnecessary nesting.
 *   All memory allocations are handled in a straightforward manner.
 *
 * Usage:
 *    1. Save this file as cnumpy_allinone.c
 *    2. Compile with:  gcc cnumpy_allinone.c -o cnumpy_allinone -lm
 *    3. Run: ./cnumpy_allinone
 *
 * Author: ChatGPT (OpenAI), customized for open source students, 2024
 * License: MIT
 * ===========================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <string.h>

// -------------------------- Struct Definition --------------------------

typedef struct {
    double *data;          // pointer to the array of doubles
    size_t size;           // length of the array
} CNumPyArray;

// -------------------------- Array Creation & Deletion --------------------------

CNumPyArray create_array(const double *initial_values, size_t array_size)
{
    CNumPyArray array;
    array.size = array_size;                           // set array length
    array.data = (double *)malloc(array_size * sizeof(double));  // allocate memory
    if (array.data == NULL)
    {
        fprintf(stderr, "Memory allocation failed.\n");
        exit(1);
    }
    if (initial_values != NULL)
    {
        memcpy(array.data, initial_values, array_size * sizeof(double)); // copy values if provided
    }
    else
    {
        memset(array.data, 0, array_size * sizeof(double)); // default fill to 0
    }
    return array;
}

CNumPyArray array_zeros(size_t array_size)
{
    return create_array(NULL, array_size); // all zeros
}

CNumPyArray array_ones(size_t array_size)
{
    CNumPyArray array = create_array(NULL, array_size);
    for (size_t index = 0; index < array_size; ++index)
    {
        array.data[index] = 1.0;                      // set each element to one
    }
    return array;
}

CNumPyArray array_full(size_t array_size, double fill_value)
{
    CNumPyArray array = create_array(NULL, array_size);
    for (size_t index = 0; index < array_size; ++index)
        array.data[index] = fill_value;                // fill with user-specified value
    return array;
}

CNumPyArray array_range(double start_value, double end_value, double step_value)
{
    // computes how many elements
    size_t count = (size_t)ceil((end_value - start_value) / step_value);
    CNumPyArray array = create_array(NULL, count);
    for (size_t index = 0; index < count; ++index)
    {
        array.data[index] = start_value + step_value * ((double)index); // calculate each value
    }
    return array;
}

CNumPyArray array_linspace(double start_value, double end_value, size_t number_values)
{
    // evenly spaced from start to end (inclusive)
    CNumPyArray array = create_array(NULL, number_values);
    if (number_values == 1)
    {
        array.data[0] = start_value;
    }
    else
    {
        for (size_t index = 0; index < number_values; ++index)
        {
            array.data[index] = start_value + (end_value - start_value) * index / (number_values - 1); // linear interpolation
        }
    }
    return array;
}

void free_array(CNumPyArray *array)
{
    if (array->data)
    {
        free(array->data);            // release memory
        array->data = NULL;
    }
    array->size = 0;                  // mark as empty
}

CNumPyArray copy_array(const CNumPyArray *array)
{
    return create_array(array->data, array->size);  // shallow copy
}

// -------------------------- Array Utilities --------------------------

void print_array(const CNumPyArray *array, int print_precision)
{
    printf("[");
    for (size_t index = 0; index < array->size; ++index)
    {
        printf("%.*f", print_precision, array->data[index]);           // print with specified digits
        if (index + 1 != array->size)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

void fill_array(CNumPyArray *array, double fill_value)
{
    for (size_t index = 0; index < array->size; ++index)
        array->data[index] = fill_value;               // fill each element
}

void reverse_array(CNumPyArray *array)
{
    size_t last_index = array->size - 1;
    for (size_t index = 0; index < array->size / 2; ++index)
    {
        double temp = array->data[index];
        array->data[index] = array->data[last_index - index];
        array->data[last_index - index] = temp;        // swap pairs from each end
    }
}

// Compare two arrays for elementwise equality
bool equal_array(const CNumPyArray *array1, const CNumPyArray *array2)
{
    if (array1->size != array2->size)
        return false;
    for (size_t index = 0; index < array1->size; ++index)
        if (array1->data[index] != array2->data[index])
            return false;
    return true;
}

// Return true if any element is nonzero
bool any_array(const CNumPyArray *array)
{
    for (size_t index = 0; index < array->size; ++index)
        if (array->data[index] != 0)
            return true;
    return false;
}

// Return true if all elements are nonzero
bool all_array(const CNumPyArray *array)
{
    for (size_t index = 0; index < array->size; ++index)
        if (array->data[index] == 0)
            return false;
    return true;
}

// Clip every value into range [min_value, max_value]
CNumPyArray clip_array(const CNumPyArray *array, double min_value, double max_value)
{
    CNumPyArray out = create_array(NULL, array->size);
    for (size_t index = 0; index < array->size; ++index)
    {
        if (array->data[index] < min_value)
            out.data[index] = min_value;
        else if (array->data[index] > max_value)
            out.data[index] = max_value;
        else
            out.data[index] = array->data[index];   // unmodified if in range
    }
    return out;
}

// Unique: return sorted unique values (slow method for demonstration)
CNumPyArray unique_array(const CNumPyArray *array)
{
    if (array->size == 0) return array_zeros(0);   // empty remains empty
    CNumPyArray temp = copy_array(array);

    // simple selection sort
    for (size_t i = 0; i < temp.size; ++i)
    {
        size_t min_index = i;
        for (size_t j = i + 1; j < temp.size; ++j)
        {
            if (temp.data[j] < temp.data[min_index])
                min_index = j;
        }
        double t = temp.data[i];
        temp.data[i] = temp.data[min_index];
        temp.data[min_index] = t;
    }
    // unique extraction, always copy first element
    double *unique_buffer = malloc(temp.size * sizeof(double));
    size_t unique_size = 1;
    unique_buffer[0] = temp.data[0];
    for (size_t index = 1; index < temp.size; ++index)
    {
        if (temp.data[index] != temp.data[index - 1])
        {
            unique_buffer[unique_size++] = temp.data[index];
        }
    }
    free_array(&temp);
    CNumPyArray res;
    res.size = unique_size;
    res.data = realloc(unique_buffer, unique_size * sizeof(double)); // resize
    return res;
}

// Sort array in-place
void sort_array(CNumPyArray *array)
{
    // simple selection sort for clarity
    for (size_t i = 0; i + 1 < array->size; ++i)
    {
        size_t min_index = i;
        for (size_t j = i + 1; j < array->size; ++j)
            if (array->data[j] < array->data[min_index])
                min_index = j;
        double t = array->data[i];
        array->data[i] = array->data[min_index];
        array->data[min_index] = t;
    }
}

// -------------------------- Element-wise Operations (Array-Array) --------------------------

void require_same_size(const CNumPyArray *array1, const CNumPyArray *array2, const char *message)
{
    if (array1->size != array2->size)
    {
        fprintf(stderr, "%s: arrays sizes not equal (%zu, %zu)\n", message, array1->size, array2->size);
        exit(1);
    }
}

CNumPyArray add_array(const CNumPyArray *array1, const CNumPyArray *array2)
{
    require_same_size(array1, array2, "add");
    CNumPyArray result = create_array(NULL, array1->size);     // allocate result
    for (size_t index = 0; index < array1->size; ++index)
        result.data[index] = array1->data[index] + array2->data[index];
    return result;
}

CNumPyArray subtract_array(const CNumPyArray *array1, const CNumPyArray *array2)
{
    require_same_size(array1, array2, "subtract");
    CNumPyArray result = create_array(NULL, array1->size);
    for (size_t index = 0; index < array1->size; ++index)
        result.data[index] = array1->data[index] - array2->data[index];
    return result;
}

CNumPyArray multiply_array(const CNumPyArray *array1, const CNumPyArray *array2)
{
    require_same_size(array1, array2, "multiply");
    CNumPyArray result = create_array(NULL, array1->size);
    for (size_t index = 0; index < array1->size; ++index)
        result.data[index] = array1->data[index] * array2->data[index];
    return result;
}

CNumPyArray divide_array(const CNumPyArray *array1, const CNumPyArray *array2)
{
    require_same_size(array1, array2, "divide");
    CNumPyArray result = create_array(NULL, array1->size);
    for (size_t index = 0; index < array1->size; ++index)
    {
        if (array2->data[index] == 0.0)
            result.data[index] = 0.0;                 // safe zero on division by zero
        else
            result.data[index] = array1->data[index] / array2->data[index];
    }
    return result;
}

CNumPyArray modulo_array(const CNumPyArray *array1, const CNumPyArray *array2)
{
    require_same_size(array1, array2, "modulo");
    CNumPyArray result = create_array(NULL, array1->size);
    for (size_t index = 0; index < array1->size; ++index)
        result.data[index] = fmod(array1->data[index], array2->data[index]);
    return result;
}

// -------------------------- Element-wise Operations (Array-Scalar) --------------------------

CNumPyArray add_scalar(const CNumPyArray *array, double value)
{
    CNumPyArray result = create_array(NULL, array->size);
    for (size_t index = 0; index < array->size; ++index)
        result.data[index] = array->data[index] + value;
    return result;
}
CNumPyArray subtract_scalar(const CNumPyArray *array, double value)
{
    CNumPyArray result = create_array(NULL, array->size);
    for (size_t index = 0; index < array->size; ++index)
        result.data[index] = array->data[index] - value;
    return result;
}
CNumPyArray multiply_scalar(const CNumPyArray *array, double value)
{
    CNumPyArray result = create_array(NULL, array->size);
    for (size_t index = 0; index < array->size; ++index)
        result.data[index] = array->data[index] * value;
    return result;
}
CNumPyArray divide_scalar(const CNumPyArray *array, double value)
{
    CNumPyArray result = create_array(NULL, array->size);
    for (size_t index = 0; index < array->size; ++index)
        result.data[index] = value == 0.0 ? 0.0 : array->data[index] / value;
    return result;
}
CNumPyArray modulo_scalar(const CNumPyArray *array, double value)
{
    CNumPyArray result = create_array(NULL, array->size);
    for (size_t index = 0; index < array->size; ++index)
        result.data[index] = fmod(array->data[index], value);
    return result;
}

// -------------------------- Element-wise Math Functions --------------------------

typedef double (*UnaryFunction)(double);

CNumPyArray apply_unary(const CNumPyArray *array, UnaryFunction f)
{
    CNumPyArray result = create_array(NULL, array->size);
    for (size_t index = 0; index < array->size; ++index)
        result.data[index] = f(array->data[index]);
    return result;
}

CNumPyArray absolute_array(const CNumPyArray *array)   { return apply_unary(array, fabs); }
CNumPyArray sin_array(const CNumPyArray *array)        { return apply_unary(array, sin); }
CNumPyArray cos_array(const CNumPyArray *array)        { return apply_unary(array, cos); }
CNumPyArray tan_array(const CNumPyArray *array)        { return apply_unary(array, tan); }
CNumPyArray asin_array(const CNumPyArray *array)       { return apply_unary(array, asin); }
CNumPyArray acos_array(const CNumPyArray *array)       { return apply_unary(array, acos); }
CNumPyArray atan_array(const CNumPyArray *array)       { return apply_unary(array, atan); }
CNumPyArray exp_array(const CNumPyArray *array)        { return apply_unary(array, exp); }
CNumPyArray log_array(const CNumPyArray *array)        { return apply_unary(array, log); }
CNumPyArray log10_array(const CNumPyArray *array)      { return apply_unary(array, log10); }
CNumPyArray sqrt_array(const CNumPyArray *array)       { return apply_unary(array, sqrt); }
CNumPyArray floor_array(const CNumPyArray *array)      { return apply_unary(array, floor); }
CNumPyArray ceil_array(const CNumPyArray *array)       { return apply_unary(array, ceil); }
CNumPyArray round_array(const CNumPyArray *array)      { return apply_unary(array, round); }

CNumPyArray pow_array(const CNumPyArray *array, double value)
{
    CNumPyArray result = create_array(NULL, array->size);
    for (size_t index = 0; index < array->size; ++index)
        result.data[index] = pow(array->data[index], value);
    return result;
}

// -------------------------- Aggregation & Statistics --------------------------

double sum_array(const CNumPyArray *array)
{
    double s = 0.0;
    for (size_t index = 0; index < array->size; ++index)
        s += array->data[index];                          // accumulate
    return s;
}

double product_array(const CNumPyArray *array)
{
    double p = 1.0;
    for (size_t index = 0; index < array->size; ++index)
        p *= array->data[index];                          // multiply over all
    return p;
}

double mean_array(const CNumPyArray *array)
{
    return sum_array(array) / array->size;                // arithmetic mean
}

double max_array(const CNumPyArray *array)
{
    double max_value = array->data[0];
    for (size_t index = 1; index < array->size; ++index)
        if (array->data[index] > max_value)
            max_value = array->data[index];               // update if larger
    return max_value;
}

double min_array(const CNumPyArray *array)
{
    double min_value = array->data[0];
    for (size_t index = 1; index < array->size; ++index)
        if (array->data[index] < min_value)
            min_value = array->data[index];
    return min_value;
}

size_t argmax_array(const CNumPyArray *array)
{
    size_t argmax_index = 0;
    double max_value = array->data[0];
    for (size_t index = 1; index < array->size; ++index)
        if (array->data[index] > max_value)
        {
            argmax_index = index;
            max_value = array->data[index];
        }
    return argmax_index;
}

size_t argmin_array(const CNumPyArray *array)
{
    size_t argmin_index = 0;
    double min_value = array->data[0];
    for (size_t index = 1; index < array->size; ++index)
        if (array->data[index] < min_value)
        {
            argmin_index = index;
            min_value = array->data[index];
        }
    return argmin_index;
}

double variance_array(const CNumPyArray *array)
{
    double mu = mean_array(array);
    double s2 = 0.0;
    for (size_t index = 0; index < array->size; ++index)
        s2 += (array->data[index] - mu) * (array->data[index] - mu);
    return s2 / array->size;
}
double std_array(const CNumPyArray *array)
{
    return sqrt(variance_array(array));    // stddev
}

// -------------------------- Linear Algebra --------------------------

double dot_array(const CNumPyArray *array1, const CNumPyArray *array2)
{
    require_same_size(array1, array2, "dot");
    double sum = 0.0;
    for (size_t index = 0; index < array1->size; ++index)
        sum += array1->data[index] * array2->data[index];
    return sum;
}

// Compute L2 norm (Euclidean)
double l2_norm(const CNumPyArray *array)
{
    double s = 0.0;
    for (size_t index = 0; index < array->size; ++index)
        s += array->data[index] * array->data[index];   // accumulate square
    return sqrt(s);
}

// -------------------------- Demo/Main --------------------------

int main(void)
{
    double values[] = {2.0, 4.0, 6.0, 8.0, 10.0};
    CNumPyArray array1 = create_array(values, 5);                                   // new array with [2,4,6,8,10]

    printf("array1 = ");
    print_array(&array1, 2);

    CNumPyArray ones = array_ones(5);                                               // [1,1,1,1,1]
    CNumPyArray array_add = add_array(&array1, &ones);                              // elementwise add
    printf("array1 + ones = ");
    print_array(&array_add, 2);

    CNumPyArray array_sin = sin_array(&array1);
    printf("sin(array1) = ");
    print_array(&array_sin, 4);

    CNumPyArray array_exp = exp_array(&array1);
    printf("exp(array1) = ");
    print_array(&array_exp, 2);

    CNumPyArray array_cube = pow_array(&array1, 3.0);                               // pow 3
    printf("array1 ^ 3 = ");
    print_array(&array_cube, 1);

    printf("Sum: %.2f\n", sum_array(&array1));
    printf("Mean: %.2f\n", mean_array(&array1));
    printf("Max: %.2f (index %zu)\n", max_array(&array1), argmax_array(&array1));
    printf("Min: %.2f (index %zu)\n", min_array(&array1), argmin_array(&array1));
    printf("Std Dev: %.6f\n", std_array(&array1));
    printf("Prod: %.2f\n", product_array(&array1));

    CNumPyArray reversed = copy_array(&array1);
    reverse_array(&reversed);    // changes in-place
    printf("Reversed array1: ");
    print_array(&reversed,2);

    CNumPyArray clipped = clip_array(&array1, 3, 8);
    printf("Clip array1 to [3, 8]: ");
    print_array(&clipped, 1);

    CNumPyArray array1_plus100 = add_scalar(&array1, 100);
    printf("array1 + 100 = ");
    print_array(&array1_plus100, 0);

    double dot_result = dot_array(&array1, &ones);
    printf("Dot product with ones: %.2f\n", dot_result);
    printf("L2 norm: %.3f\n", l2_norm(&array1));

    // Unique demo
    double duplicate_values[] = {2,2,3,4,3,5,6};
    CNumPyArray with_duplicates = create_array(duplicate_values, 7);
    CNumPyArray uniques = unique_array(&with_duplicates);
    printf("Original with duplication: ");
    print_array(&with_duplicates,0);
    printf("Unique sorted: ");
    print_array(&uniques,0);

    // Linspace demo
    CNumPyArray linsp = array_linspace(0, 1, 6);
    printf("Linspace 0..1,6: ");
    print_array(&linsp,3);

    // Range (arange) demo
    CNumPyArray arra = array_range(1, 7, 1);
    printf("Range 1..7 step 1: ");
    print_array(&arra,0);

    // Any and All
    printf("array1 any: %d, all: %d\n", any_array(&array1), all_array(&array_add));

    // Freeing everything
    free_array(&array1);
    free_array(&ones);
    free_array(&array_add);
    free_array(&array_sin);
    free_array(&array_exp);
    free_array(&array_cube);
    free_array(&reversed);
    free_array(&clipped);
    free_array(&array1_plus100);
    free_array(&with_duplicates);
    free_array(&uniques);
    free_array(&linsp);
    free_array(&arra);
    return 0;
}
