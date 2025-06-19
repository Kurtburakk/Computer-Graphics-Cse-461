# OpenGL 3D Scene Renderer

A comprehensive OpenGL-based 3D graphics application implementing modern rendering techniques with Phong lighting, texture mapping, and interactive camera controls.

## 🎯 Features

- **Modern OpenGL Rendering Pipeline** with VAO/VBO/EBO management
- **Phong Lighting Model** with ambient, diffuse, and specular components
- **Multi-texture Support** with blend factors
- **Interactive Camera System** with mouse and keyboard controls
- **Material System** with customizable properties
- **3D Object Management** with transformations and animations
- **Collision Detection** using AABB (Axis-Aligned Bounding Boxes)

## 🛠️ Technologies Used

- **OpenGL 3.3+** - Core graphics API
- **GLFW** - Window management and input handling
- **GLAD** - OpenGL function loader
- **GLM** - Mathematics library for 3D transformations
- **stb_image** - Image loading for textures
- **C++17** - Programming language

## 📁 Project Structure

```
OpenGLAssignment/
├── src/
│   ├── main.cpp          # Application entry point
│   ├── Object.cpp        # 3D object management
│   ├── Object.h          # Object class definition
│   ├── Shader.cpp        # Shader compilation and management
│   ├── Shader.h          # Shader class definition
│   ├── Camera.cpp        # Camera system implementation
│   └── Camera.h          # Camera class definition
├── shaders/
│   ├── phong.vert        # Vertex shader (Phong lighting)
│   └── phong.frag        # Fragment shader (Phong lighting)
├── textures/             # Texture assets
├── models/               # 3D model files
└── README.md
```

## 🚀 Getting Started

### Prerequisites

- **C++ Compiler** (GCC, Clang, or MSVC)
- **CMake** 3.10 or higher
- **OpenGL 3.3+** compatible graphics driver

### Dependencies

The project uses the following libraries:
- GLFW3
- GLAD
- GLM
- stb_image

### Building the Project

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Kurtburakk/Computer-Graphics-Cse-461.git
   cd Computer-Graphics-Cse-461/OpenGL/OpenGLAssignment
   ```

2. **Create build directory:**
   ```bash
   mkdir build
   cd build
   ```

3. **Configure and build:**
   ```bash
   cmake ..
   make
   ```

4. **Run the application:**
   ```bash
   ./OpenGLAssignment
   ```

## 🎮 Controls

### Camera Movement
- **W/S** - Move forward/backward
- **A/D** - Move left/right
- **Mouse** - Look around (first-person view)
- **Scroll Wheel** - Zoom in/out

### Application Controls
- **ESC** - Exit application
- **F** - Toggle fullscreen mode

## 🏗️ Architecture Overview

### Core Components

#### Object Class
Manages 3D objects with vertex data, materials, and transformations:
- **Vertex Buffer Management** (VBO/VAO/EBO)
- **Texture Loading** with fallback support
- **Material Properties** (ambient, diffuse, specular, shininess)
- **Transform Operations** (translation, rotation, scaling)
- **AABB Collision Detection**

#### Shader System
Handles shader compilation and uniform management:
- **Runtime Shader Compilation**
- **Uniform Variable Binding**
- **Error Handling and Debugging**

#### Camera System
Implements interactive first-person camera:
- **View Matrix Calculation**
- **Projection Matrix Management**
- **Mouse and Keyboard Input Processing**

### Rendering Pipeline

1. **Setup Phase:**
   - Initialize OpenGL context
   - Compile and link shaders
   - Load textures and vertex data
   - Configure vertex attributes

2. **Render Loop:**
   - Clear framebuffer
   - Update camera matrices
   - Set lighting uniforms
   - Render objects with materials
   - Swap buffers

## 🎨 Shader System

### Vertex Shader (phong.vert)
- Transforms vertices to world space
- Calculates lighting vectors
- Passes interpolated values to fragment shader

### Fragment Shader (phong.frag)
- Implements Phong lighting model
- Combines multiple textures
- Applies material properties

### Vertex Attributes
```cpp
layout (location = 0) in vec3 aPos;      // Vertex position
layout (location = 1) in vec3 aNormal;   // Vertex normal
layout (location = 2) in vec2 aTexCoords; // Texture coordinates
```

## 🔧 Key Features Implementation

### Modern OpenGL Buffer Management
```cpp
// Efficient vertex buffer setup
glGenVertexArrays(1, &VAO);
glGenBuffers(1, &VBO);
glGenBuffers(1, &EBO);

// Vertex attribute configuration
glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
```

### Material System
- **Multi-texture blending** with customizable blend factors
- **Phong material properties** (ambient, diffuse, specular, shininess)
- **Fallback texture system** for missing assets

### Performance Optimizations
- **VAO state caching** for efficient rendering
- **Static buffer allocation** for geometry data
- **Mipmap generation** for texture filtering
- **Frustum culling** preparation with AABB

## 🎓 Educational Value

This project demonstrates:
- **Modern OpenGL best practices**
- **3D mathematics in computer graphics**
- **Shader programming fundamentals**
- **Object-oriented graphics architecture**
- **Real-time rendering techniques**

## 📸 Screenshots

*Add screenshots of your rendered scenes here*

## 🐛 Known Issues

- Ensure graphics drivers support OpenGL 3.3+
- Some texture formats may require additional stb_image configuration
- Mouse sensitivity may need adjustment based on system settings

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch
3. Commit your changes
4. Push to the branch
5. Create a Pull Request

## 📄 License

This project is part of CSE 461 Computer Graphics course assignment.

## 👨‍💻 Author

**Kurtburakk**
- GitHub: [@Kurtburakk](https://github.com/Kurtburakk)
- Course: CSE 461 - Computer Graphics

## 🙏 Acknowledgments

- OpenGL community for excellent documentation
- LearnOpenGL.com for comprehensive tutorials
- stb libraries for image loading utilities
- GLM team for the mathematics library
