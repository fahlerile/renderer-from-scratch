# renderer-from-scratch

The goal of this project is to create a simple program that can transform vertex data from .obj file to an image using as little to no libraries as possible.

## Libraries Used:
- [SDL2](https://www.libsdl.org/) because I don't want to handle windows by myself. The only features of SDL2 I am using are window handling and drawing pixels.
- [fpm](https://github.com/MikeLankamp/fpm) (fixed-point math) because I don't want to reinvent a math library.

# TODO

- Add projection matrix
- Implement a Z-buffer
    - Add a z-buffer array as private member of `Window` class
    - Each time drawing a pixel, update the z-buffer for this pixel
        - Update `Window::triangle()`
        - Update `Window::line()`
- Try to improve `Window::triangle()` performance (6ms per frame with 2 color-interpolated triangles is very slow)