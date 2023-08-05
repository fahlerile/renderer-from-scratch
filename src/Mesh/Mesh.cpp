#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>

#include "Mesh.hpp"
#include "Window/Window.hpp"
#include "Light/Light.hpp"
#include "utils/Color/Color.hpp"
#include "utils/vec/vec.hpp"
#include "utils/mat/mat.hpp"

// Parse .obj file
Mesh::Mesh(std::string path)
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

// `rot_angles` in radians
void Mesh::add_position(vec3d position, vec3d rot_angles)
{
    mat4 transform(1.0);
    transform = transform.rotate(rot_angles);
    transform = transform.translate(position);

    this->model_matrices.push_back(transform);
}

void Mesh::render(Window* window, mat4& view_mat, mat4& projection_mat, std::vector<Light*>& lights)
{
    // For every model instance (for every model matrix)
    for (auto model_mat : this->model_matrices)
    {
        mat4 mvp = projection_mat * view_mat * model_mat;

        // For every face in this model
        for (int i = 0; i < this->n_faces(); i++)
        {
            vec3i face = this->face(i);

            // Vertices, world space
            // -1 because .obj is 1-indexed
            vec3d v[3];
            for (int i = 0; i < 3; i++)
                v[i] = this->vertex(face[i] - 1);

            // points on the screen, screen space, normalized device coordinates
            vec4d p[3];
            for (int i = 0; i < 3; i++)
            {
                p[i] = (mvp * (vec4d) {v[i].x, v[i].y, v[i].z, 1});

                // Perspective divide
                for (int j = 0; j < 3; j++)
                    p[i][j] = p[i][j] / p[i].w;
            }

            // calculate normal vector for lighting
            vec4d zero_to_one = p[1] - p[0];
            vec4d zero_to_two = p[2] - p[0];
            vec3d normal = ((vec3d) {zero_to_two.x, zero_to_two.y, zero_to_two.z}).cross_product({zero_to_one.x, zero_to_one.y, zero_to_one.z});
            normal = normal.normalize();

            // calculate if this triangle is visible by the
            // camera. if not, skip it (backface culling)
            // using this hard-coded vector because in screen
            // space camera is always looking in +Z direction
            float visible = normal.dot_product({0, 0, 1});
            if (visible < 0)
                continue;

            // calculate light intensities, get average color
            Color color = {0, 0, 0};
            for (auto light : lights)
                color = color + (light->get_color() * light->calculate_intensity(normal, view_mat));
            color = color / lights.size();

            window->triangle({p[2], p[1], p[0]}, color);
        }
    }
}

vec3d Mesh::vertex(int index)
{
    return this->vertices[index];
}

vec3i Mesh::face(int index)
{
    return this->faces[index];
}

int Mesh::n_faces()
{
    return this->faces.size();
}
