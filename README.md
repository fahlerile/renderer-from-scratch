# renderer-from-scratch

The goal of this project is to create a simple program that can transform vertex data from .obj file to an image using as little to no libraries as possible.

Uses right-hand coordinate system.

## Libraries Used:
- [SDL2](https://www.libsdl.org/) because I don't want to handle windows by myself. The only features of SDL2 I am using are window handling and drawing pixels.
- [fpm](https://github.com/MikeLankamp/fpm) (fixed-point math) because I don't want to reinvent a math library.

# TODO

- `Window::line()`
    - Make `Window::line()` to accept Normalized Device Coordinates (just like `Window::triangle()`)
    - Implement a Z-buffer for `Window::line()`
- Figure out the math behind perspective projection matrix
- Better light
    - Make "invisible" light pointing from the camera (to render faces that can be seen and to not render faces that cannot be seen)
    - Fix missing polygons (polygons can be seen but still are not rendered) (P.S add some kind of threshold? `if (intensity + threshold > 0) { render() }`)
    - `Light` class?
- Better camera
    - Save pointer to camera in Renderer, not the camera itself
- Try to improve `Window::triangle()` performance (6ms per frame with 2 color-interpolated triangles is very slow)
- Real-time rendering with moving camera