#pragma once
#include <string>
#include <vector>
#include "Window/Window.hpp"
#include "Texture/Texture.hpp"
#include "utils/vec/vec.hpp"
#include "utils/mat/mat.hpp"

struct face_t
{
public:
    std::vector<size_t> vertex_indices;
    std::vector<size_t> texture_coordinate_indices;
};

class Mesh
{
public:
    Mesh(std::string path, Texture *texture);

    void add_position(vec3d position, vec3d rot_angles);
    void render(Window* window, mat4& view_mat, mat4& projection_mat);

private:
    std::vector<vec3d> vertices;
    std::vector<face_t> faces;
    std::vector<vec2d> texture_coordinates;

    std::vector<mat4> model_matrices;

    Texture *texture;
};