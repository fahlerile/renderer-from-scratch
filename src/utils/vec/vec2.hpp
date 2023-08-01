#pragma once
#include <fpm/fixed.hpp>
#include <stdexcept>
#include <string>
#include <sstream>
#include <cmath>

template <typename T>
struct vec2
{
    T x = 0;
    T y = 0;

    T operator+(vec2<T> other);
    T operator-(vec2<T> other);
    T operator[](int index) const;
    T& operator[](int index);

    vec2<float> rotate(float rad, vec2<float> center);

    std::string to_string();
};

typedef vec2<int> vec2i;
typedef vec2<float> vec2f;
typedef vec2<double> vec2d;
typedef vec2<fpm::fixed_16_16> vec2fix16_16;

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