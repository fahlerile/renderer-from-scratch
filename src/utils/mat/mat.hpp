#pragma once
#include <iostream>
#include <vector>
#include "utils/vec/vec3.hpp"
#include "utils/vec/vec4.hpp"

// Struct for handling 4x4 matrices
struct mat4
{
public:
    std::vector<std::vector<double>> data;

    mat4();
    mat4(double value);

    vec4d row(size_t idx);
    vec4d column(size_t idx);

    mat4 translate(vec3d position);
    mat4 rotate(vec3d angles);
    vec4d operator*(vec4d b);
    mat4 operator*(mat4 b);

    void print();
};

// Initialize matrix with zeros (zero matrix)
inline mat4::mat4()
{
    for (int i = 0; i < 4; i++)
    {
        std::vector<double> tmp;
        for (int j = 0; j < 4; j++)
        {
            tmp.push_back(0.0);
        }
        this->data.push_back(tmp);
    }
}

// Initialize matrix with some value at diagonal (e.g 1.0 to have an identity matrix)
inline mat4::mat4(double value)
{
    for (int i = 0; i < 4; i++)
    {
        std::vector<double> tmp;
        for (int j = 0; j < 4; j++)
        {
            if (i == j)
                tmp.push_back(value);
            else
                tmp.push_back(0.0);
        }
        this->data.push_back(tmp);
    }
}

inline vec4d mat4::row(size_t idx)
{
    vec4d row = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
        row[i] = this->data[idx][i];
    return row;
}

inline vec4d mat4::column(size_t idx)
{
    vec4d column = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
        column[i] = this->data[i][idx];
    return column;
}

inline vec4d mat4::operator*(vec4d b)
{
    vec4d result = {0, 0, 0, 0};
    for (int i = 0; i < 4; i++)
        result[i] = this->row(i).dot_product(b);
    return result;
}

inline mat4 mat4::operator*(mat4 b)
{
    mat4 result;
    for (int i = 0; i < 4; i++)
    {
        vec4d row = this->row(i);
        for (int j = 0; j < 4; j++)
            result.data[i][j] = row.dot_product(b.column(j));
    }
    return result;
}

// Translate this matrix to `position`
inline mat4 mat4::translate(vec3d position)
{
    mat4 return_mat = *this;
    for (int i = 0; i < 3; i++)
        return_mat.data[i][3] = position[i];
    return return_mat;
}

// Apply rotation transformation to this matrix
// (NOTE: order of transformations: transformation currently in this matrix -> this rotation)
// `angles` - {x, y, z} in radians
// https://en.wikipedia.org/wiki/Rotation_matrix
inline mat4 mat4::rotate(vec3d angles)
{
    double alpha = angles[0];
    double beta = angles[1];
    double gamma = angles[2];

    double sin_a = std::sin(alpha);
    double sin_b = std::sin(beta);
    double sin_g = std::sin(gamma);
    double cos_a = std::cos(alpha);
    double cos_b = std::cos(beta);
    double cos_g = std::cos(gamma);

    mat4 rotation;

    // +--------------+---------------------------------+---------------------------------+---+
    // | cos(β)cos(γ) | sin(α)sin(β)cos(γ)-cos(α)sin(γ) | cos(α)sin(β)cos(γ)+sin(α)sin(γ) | 0 |
    // | cos(β)sin(γ) | sin(α)sin(β)sin(γ)+cos(α)cos(γ) | cos(α)sin(β)sin(γ)-sin(α)cos(γ) | 0 |
    // | -sin(β)      | sin(α)cos(β)                    | cos(α)cos(β)                    | 0 |
    // | 0            | 0                               | 0                               | 1 |
    // +--------------+---------------------------------+---------------------------------+---+

    rotation.data[0][0] = cos_b * cos_g;
    rotation.data[0][1] = sin_a * sin_b * cos_g - cos_a * sin_g;
    rotation.data[0][2] = cos_a * sin_b * cos_g + sin_a * sin_g;
    rotation.data[0][3] = 0;
    rotation.data[1][0] = cos_b * sin_g;
    rotation.data[1][1] = sin_a * sin_b * sin_g + cos_a * cos_g;
    rotation.data[1][2] = cos_a * sin_b * sin_g - sin_a * cos_g;
    rotation.data[1][3] = 0;
    rotation.data[2][0] = -sin_b;
    rotation.data[2][1] = sin_a * cos_b;
    rotation.data[2][2] = cos_a * cos_b;
    rotation.data[2][3] = 0;
    rotation.data[3][0] = 0;
    rotation.data[3][1] = 0;
    rotation.data[3][2] = 0;
    rotation.data[3][3] = 1;

    // applying this rotation matrix to whats already in current matrix
    return rotation * (*this);
}

// Pretty-print this matrix
inline void mat4::print()
{
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
            std::cout << this->data[i][j] << " ";
        std::cout << std::endl;
    }
}
