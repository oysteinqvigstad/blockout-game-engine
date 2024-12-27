## About
A lightweight 3D game engine crafted in C++ with OpenGL and GLSL shaders. The project consists of two main components:

### Engine Core 
A minimal, performance-focused game engine featuring:
- OpenGL-based rendering system
- GLSL shader implementation
- Low-level graphics architecture with GLM for camera and transform operations

### BlockOut Demo
The repository includes a 3D Tetris variant called BlockOut, demonstrating the engine’s capabilities. This game was developed as a 3-day exam, offering a unique top-down perspective on the classic Tetris concept.
The engine was developed in the course _Graphics Programming_ during the second year of the Bachelor’s in Programming at NTNU.


https://github.com/user-attachments/assets/7434efa4-be9a-4d90-b55f-f5a94b50cd70

#### Game Controls
##### Movement
- ←→↑↓: Move the piece in any direction
##### Piece Actions
- C: Rotate the current piece
- X: Drop the piece down one level
- Spacebar: Instantly drop piece to bottom
##### Visual settings
- T: Switch between textured and wireframe display
- I: Switch between ambient and Phong lighting effects




## Dependencies

| Library                                            | Description                                                                                                                              |
|----------------------------------------------------|------------------------------------------------------------------------------------------------------------------------------------------|
| [TCLAP](https://github.com/mirror/tclap)           | A command-line argument parser. Not relevant for the blockout demo, but is useful for passing arguments to the engine during prototyping |
| [GLFW](https://github.com/glfw/glfw)               | Used for window management and passing mouse and keyboard events from the window context                                                 |
| [GLM](https://github.com/g-truc/glm)               | Mathematics library used for abstracting matrix transformations                                                                          |
| [STB](https://github.com/nothings/stb/tree/master) | Specifically STB_Image for loading textures into memory                                                                                  |

The above libraries are pulled as submodules from GitHub and compiled automatically through CMake.
GLAD (library for dynamically loading OpenGL function pointers) is also "vendored" through CMake, targeting OpenGL 4.3. For targeting a different version, the files in `external/glad` should be replaced.

## Build Instructions

Run CMake like you would normally for your system. On a unix system it would be:

1. `cmake . -B ./build`
2. `cmake --build ./build`

And then run the program: 

3. `./build/bin/BlockOut`

## Core Engine Features

- Abstractions for **Vertex Buffer**, **Index Buffer** and **Vertex Array** management
- Custom **Shader System** with configurable vertex and fragment shaders
- **Phong Lighting**, using spotlights and point lights
- **Model class** for 3D object transformations: scaling, rotation, translation
- **LightManager** singleton for dynamically adjusting light positions, color, and attenuation
- 2D and Cubemap **Texture Management** with seemless integration of textures into shaders
- Configurable **Camera System** with support for view and projection matrix setups
- Real-time **Keyboard and Mouse Input** integration using GLFW
