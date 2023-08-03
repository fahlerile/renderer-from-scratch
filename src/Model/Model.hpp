#pragma once
#include <string>
#include <vector>
#include "utils/vec/vec.hpp"
#include "utils/mat/mat.hpp"

class Model
{
public:
    Model(std::string path);

    void add_position(vec3d position);
    void render();

    vec3i face(int index);
    vec3d vertex(int index);
    int n_faces();

private:
    std::vector<vec3d> vertices;
    std::vector<vec3i> faces;

    std::vector<mat4> model_matrices;
};