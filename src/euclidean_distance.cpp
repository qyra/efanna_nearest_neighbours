#include "euclidean_distance.hpp"

#include <math.h>
#include <immintrin.h>

inline float sum_of_squares(const int n, const float* x, const float* y)
{
    float result = 0.f;
    for(int i = 0; i < n; ++i){
        const float num = x[i] - y[i];
        result += num * num;
    }
    return result;
}

float avx2_sum_of_squares(int n, const float* x, const float* y)
{
    float result;
    //Add the vectors 8 at a time.
    __m256 eight_sums = _mm256_setzero_ps();
    for (; n>=8; n-=8) {
        const __m256 a = _mm256_loadu_ps(x);
        const __m256 b = _mm256_loadu_ps(y);
        const __m256 a_minus_b = _mm256_sub_ps(a,b);
        const __m256 a_minus_b_squared = _mm256_mul_ps(a_minus_b, a_minus_b);
        eight_sums = _mm256_add_ps(eight_sums, a_minus_b_squared);
        x+=8;
        y+=8;
    }

    //Convert 8sum into a 4sum
    __m128 four_left_regs = _mm256_extractf128_ps(eight_sums, 0);
    __m128 four_right_regs = _mm256_extractf128_ps(eight_sums, 1);
    __m128 four_sums = _mm_add_ps(four_left_regs, four_right_regs);

    for (; n>=4; n-=4) {
        const __m128 a = _mm_loadu_ps(x);
        const __m128 b = _mm_loadu_ps(y);
        const __m128 a_minus_b = _mm_sub_ps(a,b);
        const __m128 a_minus_b_squared = _mm_mul_ps(a_minus_b, a_minus_b);
        four_sums = _mm_add_ps(four_sums, a_minus_b_squared);
        x+=4;
        y+=4;
    }

    __m128 two_sums_padded = _mm_hadd_ps(four_sums, four_sums);
    __m128 one_sum_padded = _mm_hadd_ps(two_sums_padded, two_sums_padded);
    result = _mm_cvtss_f32(one_sum_padded);

    if (n)
        result += sum_of_squares(n, x, y);    // remaining 1-3 entries

    return result;
}

float norm(const Point& a, const Point& b, const int dimensions)
{
    float x[dimensions];
    float y[dimensions];
    for(int i = 0; i < dimensions; ++i){
        x[i] = a[i];
        y[i] = b[i];
    }

#ifdef __SSE__
    return sqrt(avx2_sum_of_squares(dimensions, x, y));
#else
    return sqrt(sum_of_squares(dimensions, x, y));
#endif
}

//~ float euclidean_distance(const Point& a, const Point& b, const int dimensions)
//~ {
    //~ //Euclidean Norm.
    //~ float sum_of_squares = 0;
    //~ for(int d = 0; d < dimensions; ++d){
        //~ float diff = a[d] - b[d];
        //~ sum_of_squares += diff*diff;
    //~ }
    //~ return sqrt(sum_of_squares);
//~ }
