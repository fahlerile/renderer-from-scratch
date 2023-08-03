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
