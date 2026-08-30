# Procedural Maze Explorer

A small C++20/OpenGL engine + first-person maze explorer.

## Controls

| Input             | Action                                   |
|-------------------|------------------------------------------|
| `W` / `S`         | Move forward / backward                  |
| `A` / `D`         | Strafe left / right                      |
| Mouse             | Look around while the cursor is captured |
| Left mouse button | Capture cursor                           |
| `Esc`             | Release cursor                           |
| `R`               | Regenerate the current maze size         |
| Up arrow          | Generate a larger maze                   |
| Down arrow        | Generate a smaller maze                  |

## Features

- Procedural maze generation
- First-person movement, mouse look and collision
- Textured floor and walls with ambient, point, and player spotlight lighting
- Resizable maze, minimap

## Cloning

Clone the repository together with its submodules:
```bash
git clone https://github.com/4iwen/MazeExplorer --recursive
```

If you already cloned the repository without `--recursive`, initialize the submodules with:
```bash
git submodule update --init --recursive
```

## Building
Create a build directory and configure the project with CMake:
```bash
cmake -S . -B build
cmake --build build
```

The resulting executable will be placed in:
```
bin/
```

## Releases

Prebuilt releases for Windows, Linux and macOS are available at https://github.com/4iwen/MazeExplorer/releases

## Dependencies
The project uses the following dependencies as Git submodules:
 - [GLFW](https://github.com/glfw/glfw) - window creation and input
 - [GLAD](https://github.com/4iwen/glad-gl-3.3-core) - OpenGL loader
 - [GLM](https://github.com/g-truc/glm) - mathematics library
 - [stb](https://github.com/nothings/stb) - image loading

## Assets

Wall and floor textures are from [Gritty Lowres Scifi Wall and Floor Textures](https://opengameart.org/content/gritty-lowres-scifi-wall-and-floor-textures) by CptDrunkBear, released under [CC0 1.0](https://creativecommons.org/publicdomain/zero/1.0/).

Rat model by [Poly by Google](https://poly.pizza/m/6hsesZHvcPI), licensed under [CC BY 3.0](https://creativecommons.org/licenses/by/3.0/) via Poly Pizza.
