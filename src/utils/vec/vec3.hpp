#pragma once
#include <fpm/fixed.hpp>
#include <stdexcept>

#include <iostream>  // debug

template <typename T>
struct vec3
{
public:
    T x {0};
    T y {0};
    T z {0};

    vec3<T> operator+(vec3<T> other);
    vec3<T> operator-(vec3<T> other);
    T operator[](int index) const;
    T& operator[](int index);
    vec3<T> operator-();

    operator vec3<int>() const { return {(int) this->x, (int) this->y, (int) this->z}; };
    operator vec3<float>() const { return {(float) this->x, (float) this->y, (float) this->z}; };
    operator vec3<double>() const { return {(double) this->x, (double) this->y, (double) this->z}; };
    operator vec3<fpm::fixed_24_8>() const { return vec3<fpm::fixed_24_8> {fpm::fixed_24_8(this->x), fpm::fixed_24_8(this->y), fpm::fixed_16_16(this->z)}; };

    vec3<T> cross_product(vec3<T> b);
    T dot_product(vec3<T> b);
    vec3<T> normalize();

    std::string to_string();
};

typedef vec3<int> vec3i;
typedef vec3<float> vec3f;
typedef vec3<double> vec3d;
typedef vec3<fpm::fixed_24_8> vec3fix24_8;

template <typename T>
vec3<T> vec3<T>::operator+(vec3<T> other)
{
    return {
        this->x + other.x,
        this->y + other.y,
        this->z + other.z
    };
}

template <typename T>
vec3<T> vec3<T>::operator-(vec3<T> other)
{
    return {
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
vec3<T> vec3<T>::operator-()
{
    return {
        -this->x,
        -this->y,
        -this->z
    };
}

template <typename T>
vec3<T> vec3<T>::cross_product(vec3<T> b)
{
    return vec3<T> {
        this->y * b.z - this->z * b.y,
        this->z * b.x - this->x * b.z,
        this->x * b.y - this->y * b.x
    };
}

template <typename T>
T vec3<T>::dot_product(vec3<T> b)
{
    return (this->x * b.x + this->y * b.y + this->z * b.z);
}

template <typename T>
vec3<T> vec3<T>::normalize()
{
    // casting to double to avoid ambiguity error if T = int
    double magnitude = std::sqrt(std::pow((double) this->x, 2) + std::pow((double) this->y, 2) + std::pow((double) this->z, 2));
    return {
        this->x / magnitude,
        this->y / magnitude,
        this->z / magnitude
    };
}

template <typename T>
std::string vec3<T>::to_string()
{
    return std::string("vec2(" + this->x + ", " + this->y + ", " + this->z + ")");
}
