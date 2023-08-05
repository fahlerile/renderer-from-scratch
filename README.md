# renderer-from-scratch

This is the biggest wheel I have ever reinvented.

The goal of this project is to create a simple (relatively) program that can transform vertex data from .obj file to an image using as little to no libraries as possible.

Uses right-hand coordinate system, the rotations are clockwise.

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

- Add texture interpolation
- `DirectionalLight` direction is wrong
- Figure out the math behind perspective projection matrix

- Add n-vertex polygon support in .obj files
- Add material support (.mtl)