#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>

#include "Mesh.hpp"
#include "Window/Window.hpp"
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

            // if texture coordinate
            else if (line.substr(0, 3).compare("vt ") == 0)
            {
                std::vector<double> coords;
                // !!! 4 BECAUSE THERE IS TWO SPACES IN HEAD'S .OBJ FILE !!!
                std::istringstream iss(line.substr(4));
                std::string s_coord;

                while (std::getline(iss, s_coord, ' '))
                    coords.push_back(std::stod(s_coord));

                this->texture_coordinates.push_back({coords[0], coords[1], coords[2]});
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

void Mesh::render(Window* window, mat4& view_mat, mat4& projection_mat)
{
    // For every model instance (for every model matrix)
    for (auto model_mat : this->model_matrices)
    {
        mat4 mvp = projection_mat * view_mat * model_mat;

        // For every face in this model
        for (int i = 0; i < this->n_faces(); i++)
        {
            vec3i face = this->faces[i];

            vec3d v[3];  // vertices, world space
            vec3d vt[3];  // vertex texture coordinates
            vec4d p[3];  // points in screen space, normalized device coordinates
            for (int i = 0; i < 3; i++)
            {
                // -1 because .obj is 1-indexed
                v[i] = this->vertices[face[i] - 1];
                vt[i] = this->texture_coordinates[face[i] - 1];

                p[i] = (mvp * (vec4d) {v[i].x, v[i].y, v[i].z, 1});
                // perspective divide
                for (int j = 0; j < 3; j++)
                    p[i][j] = p[i][j] / p[i].w;
            }

            // calculate normal vector for lighting
            // discarding w value of screen space vertices
            vec3d edge01 = (vec3d) {p[1].x, p[1].y, p[1].z} - (vec3d) {p[0].x, p[0].y, p[0].z};
            vec3d edge02 = (vec3d) {p[2].x, p[2].y, p[2].z} - (vec3d) {p[0].x, p[0].y, p[0].z};
            vec3d normal = edge02.cross_product(edge01).normalize();

            // calculate if this triangle is visible by the
            // camera. if not, skip it (backface culling)
            // using this hard-coded vector because in screen
            // space camera is always looking in +Z direction
            float visible = normal.dot_product({0, 0, 1});
            if (visible < 0)
                continue;

            // calculate light intensity, get color for this triangle
            // using this hard-coded light direction vector because I
            // want the camera to light up everything
            Color color = {220, 220, 220};
            vec3d light_direction = {0, 0, 1};
            double intensity = normal.dot_product(light_direction);
            color = color * intensity;

            // window->triangle(
            //     {p[2], p[1], p[0]},
            //     {color, color, color},
            //     {vt[2], vt[1], vt[0]},
            //     // add texture
            // );
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
