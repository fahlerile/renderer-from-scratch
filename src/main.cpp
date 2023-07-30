#include <iostream>
#include <vector>

#include "Image/Image.hpp"
#include "Model/Model.hpp"

int main()
{
    int width = 512;
    int height = 512;

    Image image = Image(width, height);
    Color white = Color(255, 255, 255);
    // Model head = Model("./res/models/african_head.obj");

    std::vector<std::vector<double>> vertices = {
        {0, 0, 0},
        {1, 0, 0},
        {0, 1, 0},
        {1, 1, 0}
    };
    std::vector<std::vector<int>> faces = {
        {0, 1, 2},
        {1, 2, 3},
        {0, 1, 3}
    };
    int n_faces = 3;

    for (int i = 0; i < n_faces; i++)
    {
        std::vector<int> face = faces[i];
        for (int j = 0, n = face.size(); j < n; j++)
        {
            std::vector<double> vert0 = vertices[face[j]];
            std::vector<double> vert1 = vertices[face[(j+1) % n]];

            // so no zeros in denominatior
            double z0 = (vert0[2] == 0) ? 1 : vert0[2];
            double z1 = (vert1[2] == 0) ? 1 : vert1[2];

            // https://stackoverflow.com/questions/6139451/how-can-i-convert-3d-space-coordinates-to-2d-space-coordinates
            int u0 = (vert0[0] / z0) * (width / 3);
            int v0 = (vert0[1] / z0) * (height / 3);
            int u1 = (vert1[0] / z1) * (width / 3);
            int v1 = (vert1[1] / z1) * (height / 3);

            image.line(u0, v0, u1, v1, white);
        }
    }
    image.save("render.ppm");
}