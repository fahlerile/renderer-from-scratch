#pragma once
#include <iostream>
#include <vector>
#include "utils/vec/vec3.hpp"

template <size_t N>
struct mat
{
public:
    std::vector<std::vector<double>> data;

    mat();
    mat(double value);

    mat<4> translate(vec3d position);

    void print();
};

typedef mat<2> mat2;
typedef mat<3> mat3;
typedef mat<4> mat4;

// Initialize matrix with zeros (zero matrix)
template <size_t N>
mat<N>::mat()
{
    for (int i = 0; i < N; i++)
    {
        std::vector<double> tmp;
        for (int j = 0; j < N; j++)
        {
            tmp.push_back(0.0);
        }
        this->data.push_back(tmp);
    }
}

// Initialize matrix with some value at diagonal (e.g 1.0 to have an identity matrix)
template <size_t N>
mat<N>::mat(double value)
{
    for (int i = 0; i < N; i++)
    {
        std::vector<double> tmp;
        for (int j = 0; j < N; j++)
        {
            if (i == j)
                tmp.push_back(value);
            else
                tmp.push_back(0.0);
        }
        this->data.push_back(tmp);
    }
}

// Translate this matrix to `position`
// NOTE: USE THIS ONLY FOR 4x4 MATRICES!
template <size_t N>
mat4 mat<N>::translate(vec3d position)
{
    mat4 return_mat = *this;
    for (int i = 0; i < 3; i++)
        return_mat.data[i][N-1] = position[i];
    return return_mat;
}

// Pretty-print this matrix
template <size_t N>
void mat<N>::print()
{
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; j < N; j++)
            std::cout << this->data[i][j] << " ";
        std::cout << std::endl;
    }
}
