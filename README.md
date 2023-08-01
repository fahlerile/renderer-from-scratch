# renderer-from-scratch

The goal of this project is to create a simple program that can transform vertex data from .obj file to an image using as little to no libraries as possible.

## Libraries Used:
- [SDL2](https://www.libsdl.org/) because I don't want to handle windows by myself. The only features of SDL2 I am using are window handling and drawing pixels.
- [fpm](https://github.com/MikeLankamp/fpm) (fixed-point math) because I don't want to reinvent a math library.

# TODO

- (try to) draw a model with triangles!
- Try to improve `Window::triangle()` performance (6ms per frame with 2 color-interpolated triangles is very slow)
- Implement a Z-buffer