#include <iostream>
#include <vector>

#include "Image/Image.hpp"
#include "Model/Model.hpp"

int main()
{
    vec2i dimensions = vec2i(2048, 2048);

    Image image = Image(dimensions);
    Color white = Color(255, 255, 255);
    Model head = Model("./res/models/african_head.obj");

    for (int i = 0; i < head.n_faces(); i++)
    {
        vec3i face = head.face(i);
        for (int j = 0; j < 3; j++)
        {
            // -1 because .obj is 1-indexed
            vec3d v0 = head.vertex(face[j] - 1);
            vec3d v1 = head.vertex(face[(j+1)%3] - 1);

            int x0 = (v0[0] + 1.) * dimensions.x / 2.;
            int y0 = (v0[1] + 1.) * dimensions.y / 2.;
            int x1 = (v1[0] + 1.) * dimensions.x / 2.;
            int y1 = (v1[1] + 1.) * dimensions.y / 2.;

            // because sometimes out of bounds
            if (x0 == 0 || y0 == 0 || x1 == 0 || y1 == 0)
                image.line(vec2i(x0, y0), vec2i(x1, y1), white);
            else
                image.line(vec2i(x0 - 1, y0 - 1), vec2i(x1 - 1, y1 - 1), white);
        }
    }

    image.flip_vertically();
    image.save("render.ppm");
}