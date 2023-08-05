#pragma once
#include <string>
#include <vector>
#include "Window/Window.hpp"
#include "Light/Light.hpp"
#include "utils/vec/vec.hpp"
#include "utils/mat/mat.hpp"

class Mesh
{
public:
    Mesh(std::string path);

    void add_position(vec3d position, vec3d rot_angles);
    void render(Window* window, mat4& view_mat, mat4& projection_mat, std::vector<Light*> &lights);

    vec3i face(int index);
    vec3d vertex(int index);
    int n_faces();

private:
    std::vector<vec3d> vertices;
    std::vector<vec3i> faces;

    std::vector<mat4> model_matrices;
};