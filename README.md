# renderer-from-scratch

The goal of this project is to create a simple (relatively) program that can transform vertex data from .obj file to an image using as little to no libraries as possible (that's the reason why I don't use GLM).

Uses right-hand coordinate system.

## Libraries Used:
- [SDL2](https://www.libsdl.org/) because I don't want to handle windows by myself. I use SDL2 just to open a window and draw pixels to it.
- [fpm](https://github.com/MikeLankamp/fpm) (fixed-point math) because I don't want to write a ton of boilerplate code just to operate on fixed-point numbers.

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

- whats fucking wrong with coordinate system???
- Camera back-face culling ignores view matrix (camera rotation)
- Figure out the math behind perspective projection matrix
- Add function to add additional light sources, remove light from camera (Some kind of `Light` class?)
    - PROBLEM: find a way to store childen of one class in `std::vector`. currently everything is treated as base useless Light objects
- Try to improve rendering performance
    - OpenCL?
    - Real-time rendering with moving camera
- Add texture interpolation
- Add n-vertex polygon support in .obj files
- Add material support (.mtl)