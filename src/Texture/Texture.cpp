#include <fstream>
#include <cassert>
#include "utils/Color/Color.hpp"
#include "Texture.hpp"

// Supports only PPM images
// https://ru.wikipedia.org/wiki/Portable_anymap for format reference
Texture::Texture(std::string path)
{
    std::ifstream file(path, std::ios::binary);
    assert(file.is_open());

    // read from file, assert needed things
    std::string line;

    std::getline(file, line);  // get line with P6
    assert(line.compare("P6") == 0);

    // skip comments
    do
    {
        std::getline(file, line);
    } while (line[0] == '#');

    // at this moment there is a line with (width, height) in `line`
    assert(line.find(' ') != std::string::npos);  // assert there is at least a single space
    std::string s_width, s_height;
    std::istringstream iss(line);
    iss >> s_width >> s_height;
    this->width = std::stoi(s_width);
    this->height = std::stoi(s_height);

    std::getline(file, line);  // get line with 255
    assert(std::stoi(line) == 255);

    std::getline(file, line);  // get line with image data
    std::vector<Color> temp;
    for (int i = 0, n = line.length() - 2; i < n; i += 3)  // i - pixel index
    {
        Color color = {
            (unsigned char) line[i],
            (unsigned char) line[i+1],
            (unsigned char) line[i+2]
        };

        temp.push_back(color);
        if (i % width == 0)
        {
            std::vector<Color> image_row = temp;
            this->data.push_back(image_row);
            temp.clear();
        }
    }

    file.close();
}

Color Texture::get_color_from_uv(vec2d uv)
{
    // u = x
    // v = y
    // https://learnopengl.com/img/getting-started/tex_coords.png

    int x_coord = std::round(this->width * uv.x);
    int y_coord = this->height - std::round(this->height * uv.y);

    // x_coord = (x_coord >= width) ? width - 1 : x_coord;
    // y_coord = (y_coord >= height) ? height - 1 : y_coord;

    return this->data[y_coord][x_coord];
}
