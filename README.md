# renderer-from-scratch

This is the biggest wheel I have ever reinvented.

The goal of this project is to create a simple (relatively) program that can transform vertex data from .obj file to an image using as little to no libraries as possible.

Uses right-hand coordinate system, the rotations are clockwise.

## Libraries Used:
- [SDL2](https://www.libsdl.org/)
- [fpm](https://github.com/MikeLankamp/fpm) (fixed-point math)

# Build

```bash
git clone https://github.com/fahlerile/renderer-from-scratch.git
cd renderer-from-scratch
mkdir build
cd build
cmake ..
make
cd bin
./renderer.exe
```

# TODO

- Add texture interpolation
    - [x] Update .obj parser to parse `vt` lines
    - [x] Interpolate `u v` coordinates inside the triangle
    - [x] Add `Texture` class and pass it to triangle function
    - [x] Color the pixels in rasterization stage according to this texture
    - [x] Fix image artifacts in `test_texture.ppm` box (bug in PPM loader?)
    - [x] Create two separate functions for colored triangle and for textured triangle
    - Save not only the indices of vertices in `Mesh::faces`, but indices of texture coordinates too! (and normals later) (some `face_t` structure with `std::vectors` inside?)
        - Handle `.obj`'s 1-based indexing in parsing stage!
    - Render a textured cube
    - Render a head with textures!
- Fix color interpolation in `Window::line()`
- Gouraud shading
    - Update .obj parser to parse `vn` lines
    - Transform these normal vectors along with vertices
    - Calculate color not to triangles, but to vertices and interpolate between these colors in each triangle
    - If there is no `vn` lines, calculate vertex normals to keep program running
- Shaders
    - Unify `colored_triangle` and `textured_triangle` (they are only different in fragment shader)
- Lightning

- Triangulate n-vertex polygons in .obj files
- Materials? (.mtl)