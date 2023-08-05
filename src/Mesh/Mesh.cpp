#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>

#include "Mesh.hpp"
#include "Color/Color.hpp"
#include "Window/Window.hpp"
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

void Mesh::render(Window* window, mat4& view_mat, mat4& projection_mat, vec4d& camera_front)
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

            // calculate normal vector for lighting
            vec3d normal = (v[2]-v[0]).cross_product(v[1]-v[0]);
            normal = normal.normalize();

            // points on the screen, screen space, normalized device coordinates
            // cast to needed type, discard z coordinate and scale coordinates
            vec4fix24_8 p[3];
            for (int i = 0; i < 3; i++)
            {
                p[i] = (vec4fix24_8) (mvp * (vec4d) {v[i].x, v[i].y, v[i].z, 1});

                // Perspective divide
                for (int j = 0; j < 3; j++)
                    p[i][j] = p[i][j] / p[i].w;
            }

            // calculate light intensity
            Color light_color = {255, 255, 255};
            float visible = normal.dot_product({camera_front.x, camera_front.y, camera_front.z});

            // backface culling (rendering only faces with light)
            if (visible > 0)
            {
                Color color = light_color * visible;
                window->triangle({p[2], p[1], p[0]}, color);
            }
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
