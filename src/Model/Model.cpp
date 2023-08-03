#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>

#include "Model.hpp"
#include "Color/Color.hpp"
#include "utils/vec/vec.hpp"
#include "utils/mat/mat.hpp"

// Parse .obj file
Model::Model(std::string path)
{
    std::ifstream file(path);
    if (file.is_open())
    {
        std::string line;

        while (std::getline(file, line))
        {
            // if vertex
            if (line.substr(0, 2).compare("v ") == 0)
            {
                std::vector<double> coords;
                std::istringstream iss(line.substr(2));
                std::string s_coord;

                while (std::getline(iss, s_coord, ' '))
                    coords.push_back(std::stod(s_coord));

                this->vertices.push_back({coords[0], coords[1], coords[2]});
            }

            // if face
            else if (line.substr(0, 2).compare("f ") == 0)
            {
                std::vector<int> indices;
                std::istringstream iss(line.substr(2));
                std::string s_triple;

                while (std::getline(iss, s_triple, ' '))
                    indices.push_back(std::stoi(s_triple.substr(0, s_triple.find('/'))));

                this->faces.push_back({indices[0], indices[1], indices[2]});
            }
        }
    }
    else
        throw std::invalid_argument("Failed to open file " + path);
}

void Model::add_position(vec3d position)
{
    mat4 translation_mat(1.0);
    translation_mat = translation_mat.translate(position);
    this->model_matrices.push_back(translation_mat);
}

// TODO: Add model, view and projection matrices
void Model::render()
{
    // For every model instance (for every model matrix)
    for (auto model_mat : this->model_matrices)
    {
        // For every face in this model
        for (int i = 0; i < this->n_faces(); i++)
        {
            vec3i face = this->face(i);

            // TODO: Add model matrix multiplication here
            // vertices, world space
            // -1 because .obj is 1-indexed
            vec3d v[3] = {
                this->vertex(face[0] - 1),
                this->vertex(face[1] - 1),
                this->vertex(face[2] - 1)
            };

            // calculate normal vector for lighting
            vec3d normal = (v[2]-v[0]).cross_product(v[1]-v[0]);
            normal = normal.normalize();

            // TODO: Add projection matrix here
            // points on the screen, screen space
            // cast to needed type, discard z coordinate and scale coordinates
            vec2fix24_8 p[3] = {
                {
                    (fpm::fixed_24_8(v[0].x) + 1) * dimensions.x / 2,
                    (fpm::fixed_24_8(v[0].y) + 1) * dimensions.y / 2
                },
                {
                    (fpm::fixed_24_8(v[1].x) + 1) * dimensions.x / 2,
                    (fpm::fixed_24_8(v[1].y) + 1) * dimensions.y / 2
                },
                {
                    (fpm::fixed_24_8(v[2].x) + 1) * dimensions.x / 2,
                    (fpm::fixed_24_8(v[2].y) + 1) * dimensions.y / 2
                }
            };

            // transform coordinates to see not a flipped image
            p[0] = {p[0].x, -p[0].y};
            p[0].y += dimensions.y - 1;
            p[1] = {p[1].x, -p[1].y};
            p[1].y += dimensions.y - 1;
            p[2] = {p[2].x, -p[2].y};
            p[2].y += dimensions.y - 1;

            // calculate light intensity
            vec3d light = {0, 0, 1};
            Color light_color = {255, 255, 255};
            float intensity = normal.dot_product(light);

            // TODO: pass depths to the triangle function and implement z_buffer there
            // backface culling (rendering only faces with light)
            if (intensity > 0)
            {
                Color color = light_color * intensity;
                window.triangle({p[2], p[1], p[0]}, color);
            }
        }
    }
}

vec3d Model::vertex(int index)
{
    return this->vertices[index];
}

vec3i Model::face(int index)
{
    return this->faces[index];
}

int Model::n_faces()
{
    return this->faces.size();
}
