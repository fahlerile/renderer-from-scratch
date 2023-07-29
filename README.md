# renderer-from-scratch

The goal of this project is to create a simple program that can transform vertex data to an image.

# TODO

- Add `Image` class
    - Header
    - Constructor
    - Destructor
    - `set(vec2 position, vec3 color)` method (set the pixel at `position` to `color` color (RGB))
    - `save(std::string path)` method (save the image to a file)
- Implement Bresenham's line algorithm (create a function to draw a line)
    - ...
- Read an `.obj` file and render it in wireframe mode (lines only)
- Add function to draw triangles