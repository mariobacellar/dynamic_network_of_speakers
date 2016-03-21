#include "relative_weight_factor.hpp"
#include <iostream>
#include <stdio.h>
#include <vector>

/* Debug MACRO */
//#define MAIN_DEBUG
#ifdef MAIN_DEBUG
#define D(call) (call)
#else
#define D(call)
#endif

template <class T>
relative_weight_factor::rwf<T>::rwf (std::vector<T> vec, int head, int tail)
: _factors (vec), _steps (head - tail) {
}

template <class T> relative_weight_factor::rwf<T>::~rwf () {
}

template <class T>
std::vector<T> relative_weight_factor::rwf<T>::get_relative_weight_factor () {
    std::vector<T> vec;
    T sum          = _sum (_factors);
    T sum_inv      = _sum_inv (_factors);
    T step_per_fac = _steps / sum_inv;
    D (std::cout << "Sum: " << sum << "Sum inv: " << sum_inv
                 << "Steps/fac: " << step_per_fac << std::endl);
    for (auto i : _factors) {
        vec.push_back (
        (sum - i) * (step_per_fac)); // Add: (sum-val)*(step per factor)
        D (std::cout << "Fac: " << i << " InvFac: " << sum - i << std::endl);
    }
    return vec;
}

template <class T> T relative_weight_factor::rwf<T>::_sum (std::vector<T> vec) {
    T sum = 0;
    for (auto i : vec) {
        sum += i;
    }
    D (std::cout << "Sum: " << sum << std::endl);
    return sum;
}

template <class T>
T relative_weight_factor::rwf<T>::_sum_inv (std::vector<T> vec) {
    T sum     = _sum (vec);
    T sum_inv = 0;
    for (auto i : vec) {
        sum_inv += sum - i;
    }
    D (std::cout << "Sum inv: " << sum_inv << std::endl);
    return sum_inv;
}

template <class T>
void relative_weight_factor::rwf<T>::set_head_tail (int head, int tail) {
    _steps = head - tail;
}

template <class T>
void relative_weight_factor::rwf<T>::set_factors (std::vector<T> vec) {
    _factors = vec;
}
