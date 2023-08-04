# renderer-from-scratch

The goal of this project is to create a simple program that can transform vertex data from .obj file to an image using as little to no libraries as possible.

## Libraries Used:
- [SDL2](https://www.libsdl.org/) because I don't want to handle windows by myself. The only features of SDL2 I am using are window handling and drawing pixels.
- [fpm](https://github.com/MikeLankamp/fpm) (fixed-point math) because I don't want to reinvent a math library.

# TODO

- Figure out the math behind perspective projection matrix
- Clean up code
    - Clean up data type madness in `Window` class
- `Window::line()`
    - Make `Window::line()` to accept Normalized Device Coordinates (just like `Window::triangle()`)
    - Implement a Z-buffer for `Window::line()`
- Better light?
- Try to improve `Window::triangle()` performance (6ms per frame with 2 color-interpolated triangles is very slow)