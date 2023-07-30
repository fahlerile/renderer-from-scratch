#pragma once
#include <string>
#include <vector>

struct Color
{
    unsigned char red = 0;
    unsigned char green = 0;
    unsigned char blue = 0;

    Color();
    Color(unsigned char red, unsigned char green, unsigned char blue);
};

class Image
{
public:
    Image(unsigned int width, unsigned int height);

    void set(unsigned int x, unsigned int y, Color color);
    void line(int x0, int y0,
              int x1, int y1,
              Color color);
    void flip_vertically();
    void save(std::string path);

private:
    std::vector<std::vector<Color>> data;
    unsigned int width;
    unsigned int height;
};