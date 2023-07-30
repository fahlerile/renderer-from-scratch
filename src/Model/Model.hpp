#pragma once
#include <string>

class Model
{
public:
    Model(std::string path);

    std::vector<int> face(int index);
    std::vector<double> vertex(int index);
    int n_faces();

private:
    std::vector<std::vector<double>> vertices;
    std::vector<std::vector<int>> faces;
};