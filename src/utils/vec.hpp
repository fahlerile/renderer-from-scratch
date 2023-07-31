#pragma once
#include <stdexcept>

template <typename T>
struct vec2
{
    T x = 0;
    T y = 0;
    T operator[](int index) const;
    T& operator[](int index);
};

typedef vec2<int> vec2i;
typedef vec2<float> vec2f;
typedef vec2<double> vec2d;

// ============================================

template <typename T>
struct vec3
{
    T x = 0;
    T y = 0;
    T z = 0;
    T operator[](int index) const;
    T& operator[](int index);
};

typedef vec3<int> vec3i;
typedef vec3<float> vec3f;
typedef vec3<double> vec3d;

// ============================================

template <typename T>
T vec2<T>::operator[](int index) const
{
    if (index == 0) return this->x;
    else if (index == 1) return this->y;
    throw std::invalid_argument("vec2 invalid index " + index);
}

template <typename T>
T& vec2<T>::operator[](int index)
{
    if (index == 0) return this->x;
    else if (index == 1) return this->y;
    throw std::invalid_argument("vec2 invalid index " + index);
}

// ============================================

template <typename T>
T vec3<T>::operator[](int index) const
{
    if (index == 0) return this->x;
    else if (index == 1) return this->y;
    else if (index == 2) return this->z;
    throw std::invalid_argument("vec3 invalid index " + index);
}

template <typename T>
T& vec3<T>::operator[](int index)
{
    if (index == 0) return this->x;
    else if (index == 1) return this->y;
    else if (index == 2) return this->z;
    throw std::invalid_argument("vec3 invalid index " + index);
}
