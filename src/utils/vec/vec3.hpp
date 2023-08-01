#pragma once
#include <fpm/fixed.hpp>
#include <stdexcept>

template <typename T>
struct vec3
{
    T x = 0;
    T y = 0;
    T z = 0;

    T operator+(vec3<T> other);
    T operator-(vec3<T> other);
    T operator[](int index) const;
    T& operator[](int index);

    std::string to_string();
};

typedef vec3<int> vec3i;
typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<fpm::fixed_16_16> vec3fix16_16;

template <typename T>
T vec3<T>::operator+(vec3<T> other)
{
    return vec3<T> {
        this->x + other.x,
        this->y + other.y,
        this->z + other.z
    };
}

template <typename T>
T vec3<T>::operator-(vec3<T> other)
{
    return vec3<T> {
        this->x - other.x,
        this->y - other.y,
        this->z - other.z
    };
}

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

template <typename T>
std::string vec3<T>::to_string()
{
    return std::string("vec2(" + this->x + ", " + this->y + ", " + this->z + ")");
}
