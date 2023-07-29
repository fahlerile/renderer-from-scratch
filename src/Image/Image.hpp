#pragma once
#include <string>

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
    ~Image();

    void set(unsigned int x, unsigned int y, Color color);
    void save(std::string path);

private:
    Color** data = nullptr;
    unsigned int width;
    unsigned int height;
};