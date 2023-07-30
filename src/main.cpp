#include <iostream>
#include <vector>

#include "Image/Image.hpp"
#include "Model/Model.hpp"

int main()
{
    int width = 2048;
    int height = 2048;

    Image image = Image(width, height);
    Color white = Color(255, 255, 255);
    Model head = Model("./res/models/african_head.obj");

    for (int i = 0; i < head.n_faces(); i++)
    {
        std::vector<int> face = head.face(i);
        for (int j = 0; j < 3; j++)
        {
            // -1 because .obj is 1-indexed
            std::vector<double> v0 = head.vertex(face[j] - 1);
            std::vector<double> v1 = head.vertex(face[(j+1)%3] - 1);

            int x0 = (v0[0] + 1.) * width / 2.;
            int y0 = (v0[1] + 1.) * height / 2.;
            int x1 = (v1[0] + 1.) * width / 2.;
            int y1 = (v1[1] + 1.) * height / 2.;

            // because sometimes out of bounds
            if (x0 == 0 || y0 == 0 || x1 == 0 || y1 == 0)
                image.line(x0, y0, x1, y1, white);
            else
                image.line(x0 - 1, y0 - 1, x1 - 1, y1 - 1, white);
        }
    }

    image.flip_vertically();
    image.save("render.ppm");
}