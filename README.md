# Maze Explorer

## Cloning

Clone the repository together with its submodules:
```bash
git clone https://githgub.com/4iwen/MazeExplorer --recursive
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

## Dependencies
The project uses the following dependencies as Git submodules:
 - [GLFW](https://github.com/glfw/glfw) - window creation and input
 - [GLAD](https://github.com/4iwen/glad-gl-3.3-core) - OpenGL loader