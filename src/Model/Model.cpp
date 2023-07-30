#include <fstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <sstream>

#include "Model.hpp"

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

                this->vertices.push_back(coords);
            }

            // if face
            else if (line.substr(0, 2).compare("f ") == 0)
            {
                std::vector<int> indices;
                std::istringstream iss(line.substr(2));
                std::string s_triple;

                while (std::getline(iss, s_triple, ' '))
                    indices.push_back(std::stoi(s_triple.substr(0, s_triple.find('/'))));

                this->faces.push_back(indices);
            }
        }
    }
    else
        throw std::invalid_argument("Failed to open file " + path);
}

std::vector<double> Model::vertex(int index)
{
    return this->vertices[index];
}

std::vector<int> Model::face(int index)
{
    return this->faces[index];
}

int Model::n_faces()
{
    return this->faces.size();
}
