#pragma once
#include <fpm/fixed.hpp>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cmath>

template <typename T>
struct vec2
{
public:
    T x {0};
    T y {0};

    T operator+(vec2<T> other);
    T operator-(vec2<T> other);
    T operator[](int index) const;
    T& operator[](int index);
    vec2<T> operator-();

    operator vec2<int>() const { return {(int) this->x, (int) this->y}; };
    operator vec2<float>() const { return {(float) this->x, (float) this->y}; };
    operator vec2<double>() const { return {(double) this->x, (double) this->y}; };
    operator vec2<fpm::fixed_24_8>() const { return {fpm::fixed_24_8(this->x), fpm::fixed_24_8(this->y)}; };

    vec2<float> rotate(float rad, vec2<float> center);
    std::string to_string();
};

typedef vec2<int> vec2i;
typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<fpm::fixed_24_8> vec2fix24_8;

template <typename T>
T vec2<T>::operator+(vec2<T> other)
{
    return vec2<T> {
        this->x + other.x,
        this->y + other.y
    };
}

template <typename T>
T vec2<T>::operator-(vec2<T> other)
{
    return vec2<T> {
        this->x - other.x,
        this->y - other.y
    };
}

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

template <typename T>
vec2<T> vec2<T>::operator-()
{
    return {
        -this->x,
        -this->y
    };
}

template <typename T>
vec2f vec2<T>::rotate(float rad, vec2f center)
{
    // the idea is to place `center` at the origin of coordinate system,
    // then rotate the vector and place it to the same exact position as it was before
    vec2f temp = {this->x - center.x, this->y - center.y};
    vec2f result = {
        (temp.x * std::cos(rad)) - (temp.y * std::sin(rad)),
        (temp.x * std::sin(rad)) + (temp.y * std::cos(rad))
    };
    result.x += center.x;
    result.y += center.y;
    return result;
}

template <typename T>
std::string vec2<T>::to_string()
{
    std::ostringstream iss;
    iss << std::string("vec2(") << this->x << ", " << this->y << ")";
    return iss.str();
}