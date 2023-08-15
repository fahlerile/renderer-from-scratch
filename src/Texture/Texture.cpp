#include <fstream>
#include <cassert>
#include "utils/Color/Color.hpp"
#include "Texture.hpp"

#include <iostream>

// Supports only PPM images
// https://ru.wikipedia.org/wiki/Portable_anymap for format reference
Texture::Texture(std::string path)
{
    std::ifstream file;
    file.open(path, std::ios::binary);
    assert(!file.fail());

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

    // get the image data (until EOF)
    int end_of_header_pos = file.tellg();
    file.seekg(0, file.end);
    int length = (int) file.tellg() - end_of_header_pos;
    file.seekg(end_of_header_pos, file.beg);
    char *image_data = new char[length];
    file.read(image_data, length);

    std::vector<Color> image_row;
    for (int i = 0, n = length - 2; i < n; i += 3)  // i - byte index, each pixel consists of 3 bytes
    {
        Color color = {
            (uint8_t) image_data[i],
            (uint8_t) image_data[i+1],
            (uint8_t) image_data[i+2]
        };

        image_row.push_back(color);
        if (((i / 3) + 1) % width == 0)
        {
            this->data.push_back(image_row);
            image_row.clear();
        }
    }

    file.close();
    delete[] image_data;
}

Color Texture::get_color_from_uv(vec2d uv)
{
    // u = x
    // v = y
    // https://learnopengl.com/img/getting-started/tex_coords.png

    int x_coord = std::round((this->width - 1) * uv.x);                        // [0, width)
    int y_coord = (this->height - 1) - std::round((this->height - 1) * uv.y);  // [0, height)

    return this->data[y_coord][x_coord];
}
