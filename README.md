# slimey

This project is inspired by the [slime simulation video](https://www.youtube.com/watch?v=X-iSQQgOd1A) put out by Sebastian Lague. It will simulate the movement of slime mould using C++ and OpenGL shaders.

## Pre-commit

This repository is equipped with pre-commit for automatic style and linting enforcement. it should be installed and activated appropriately. Run the following from the root of the repo once pre-commit is installed.

``` bash

pre-commit install

```

## Setup

### Git Submodules

From the root of the repo run

``` bash

git submodule update --init --recursive

```

### Dependencies

``` bash

libgl1-mesa-dev
libglew-dev
libwayland-dev
libxkbcommon-dev
libxrandr-dev
libxinerama-dev
libxcursor-dev
libxi-dev

```

## Resources

- [Video series by The Cherno](https://youtube.com/playlist?list=PLlrATfBNZ98foTJPJ_Ev03o2oq3-GGOS2&si=NihKB6x_lgSP3dCN)
- [This tutorial series](http://www.opengl-tutorial.org/)
- [OpenGL ref pages](https://registry.khronos.org/OpenGL/index_gl.php)
