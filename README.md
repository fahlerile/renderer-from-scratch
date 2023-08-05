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

- Is something wrong with coordinate system? (rotations, translations)
- Light misbehaves?
- Figure out the math behind perspective projection matrix
- Try to improve rendering performance
    - OpenCL?
    - Try real-time rendering with moving camera
- Add texture interpolation
- Add n-vertex polygon support in .obj files
- Add material support (.mtl)