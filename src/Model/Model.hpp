#pragma once
#include <string>
#include <vector>
#include "utils/vec.hpp"

class Model
{
public:
    Model(std::string path);

    vec3i face(int index);
    vec3d vertex(int index);
    int n_faces();

private:
    std::vector<vec3d> vertices;
    std::vector<vec3i> faces;
};