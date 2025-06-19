# Ray Tracer - CSE 461 Programming Assignment 1

A comprehensive C++ ray tracer implementation that renders 3D scenes with realistic lighting, shadows, reflections, and texture mapping using XML scene description files.

## 🎯 Features

- **Ray-Triangle Intersection** with efficient algorithms
- **Phong Lighting Model** (ambient, diffuse, specular)
- **Multiple Light Sources** (point lights, triangular area lights)
- **Material System** with reflection and texture support
- **Shadow Calculation** with proper offset handling
- **Multi-threading Support** for performance optimization
- **XML Scene Parser** for flexible scene description
- **Texture Mapping** with blend factor control
- **Recursive Ray Tracing** with configurable depth

## 🛠️ Technologies Used

- **C++11** - Core programming language
- **XML Parser** - Scene description file parsing
- **Image I/O Libraries** - Texture loading and output generation
- **Multi-threading** - Performance optimization
- **Linear Algebra** - 3D mathematics and transformations

## 📁 Project Structure

```
PA1/
├── cghw1/
│   ├── src/              # Source code files
│   ├── include/          # Header files
│   ├── examplexml/       # Example scene files
│   │   └── example.xml   # Sample scene description
│   ├── textures/         # Texture image files
│   ├── Makefile          # Build configuration
│   └── program           # Compiled executable
├── RunStep.txt           # Compilation and execution instructions
└── README.md
```

## 🚀 Getting Started

### Prerequisites

- **C++11 compatible compiler** (GCC, Clang)
- **XML parsing library** (TinyXML, RapidXML, or similar)
- **Image processing library** (for texture loading)
- **Make** build system

### Building and Running

1. **Clone the repository:**
   ```bash
   git clone https://github.com/Kurtburakk/Computer-Graphics-Cse-461.git
   cd Computer-Graphics-Cse-461/RayTracer/PA1/cghw1
   ```

2. **Clean and build:**
   ```bash
   make clean
   make
   ```

3. **Run the ray tracer:**
   ```bash
   ./program your_pathname/cghw1/examplexml/example.xml
   ```

   Replace `your_pathname` with the actual path to the project directory.

4. **Test with different scenes:**
   ```bash
   ./program path/to/your/scene.xml
   ```

## 📄 Scene Description Format

The ray tracer uses XML files to describe 3D scenes. Here's the structure:

### Basic Scene Structure

The XML format defines all scene elements including camera, lights, materials, and geometry. Key sections include maxraytracedepth for recursion control, background color for missed rays, camera parameters (position, gaze, up, nearplane, distance, resolution), and comprehensive lighting system.

### Lighting Configuration

**Ambient Light:** Global illumination for shadowed areas  
**Point Light:** Positioned light sources with intensity values  
**Triangular Area Light:** Planar directional lights defined by triangle vertices with computed direction from cross product

### Material System

Materials define surface properties including ambient, diffuse, and specular coefficients, mirror reflectance for recursive reflections, phong exponent for specular highlighting, and texture factor for blending between material color and texture (0.0 = no texture, 1.0 = pure texture).

### Geometry Format

**Vertex Data:** Lists 3D coordinates for all vertices in the scene  
**Texture Data:** UV coordinates for texture mapping  
**Normal Data:** Surface normals for lighting calculations  
**Mesh Definition:** Triangular faces defined by vertex/texture/normal indices in counter-clockwise order (vertex_id/texture_id/normal_id format)

## 🏗️ Implementation Features

### Ray Tracing Pipeline

1. **Ray Generation:** Cast rays from camera through each pixel
2. **Intersection Testing:** Ray-triangle intersection using efficient algorithms
3. **Shading Calculation:** Phong lighting model implementation
4. **Shadow Rays:** Cast rays to light sources for shadow determination
5. **Reflection Rays:** Recursive ray tracing for mirror reflections
6. **Color Composition:** Combine shading, texture, and reflection contributions

### Performance Optimizations

- **Multi-threading:** Parallel pixel processing for faster rendering
- **Efficient Data Structures:** Optimized ray-triangle intersection
- **Shadow Ray Optimization:** Proper surface offset to avoid self-intersection
- **Early Ray Termination:** Maximum ray depth control

### Key Algorithms

**Ray-Triangle Intersection:**
- Möller-Trumbore algorithm or barycentric coordinate method
- Efficient triangle mesh traversal

**Phong Shading:**
- Ambient + Diffuse + Specular lighting components
- Normal interpolation for smooth shading

**Shadow Calculation:**
- Shadow ray casting with epsilon offset
- Multiple light source support

## 🎨 Advanced Features

### Texture Mapping
- UV coordinate interpolation
- Texture blending with material properties
- Support for various image formats

### Material System
- Multiple material support per scene
- Mirror reflection with configurable reflectance
- Texture factor blending (0.0 = no texture, 1.0 = pure texture)

### Multi-threading Architecture
- Thread pool for pixel processing
- Load balancing across available cores
- Configurable thread count

## 📊 Performance Considerations

The implementation focuses on efficiency:

- **Time Complexity:** O(n) per ray where n = number of triangles
- **Space Complexity:** O(m) where m = scene geometry size
- **Threading:** Scales with available CPU cores
- **Memory Usage:** Optimized data structures for large scenes

### Performance Tips

- Use appropriate ray tracing depth (typically 3-8)
- Optimize triangle count for complex meshes
- Enable multi-threading for large images
- Consider scene complexity vs. quality trade-offs

## 🔧 Code Architecture

### Core Components

- **Ray Class:** Handles ray generation and intersection testing
- **Material System:** Manages surface properties and texture blending
- **Triangle Mesh:** Efficient geometry representation with normals and UV coordinates  
- **Scene Management:** Organizes cameras, lights, and geometry
- **Lighting System:** Point lights and triangular area lights support

## 📸 Example Scenes

The project includes example XML scenes demonstrating:

- **Basic Cube:** Simple geometry with texture mapping
- **Multi-object Scenes:** Complex arrangements with multiple materials
- **Lighting Demos:** Various light source configurations
- **Reflection Tests:** Mirror surfaces and recursive reflections

## 🐛 Common Issues & Solutions

### Compilation Issues
- Ensure C++11 compiler support
- Check XML library dependencies
- Verify image I/O library installation

### Runtime Problems
- **Black Output:** Check camera positioning and scene bounds
- **Performance Issues:** Reduce ray depth or image resolution
- **Shadow Artifacts:** Adjust surface offset epsilon value

### Scene File Errors
- Validate XML syntax
- Check vertex/face index ranges
- Ensure material IDs are properly referenced

## 📈 Performance Benchmarks

Example rendering times (800x800 image):
- **Simple Scene (100 triangles):** ~2-5 seconds
- **Complex Scene (1000+ triangles):** ~30-60 seconds
- **High-quality (recursive depth 6+):** ~2-5 minutes

*Performance varies based on scene complexity, ray depth, and hardware*

## 🎓 Educational Value

This project demonstrates:
- **Ray Tracing Fundamentals:** Core algorithms and principles
- **3D Graphics Mathematics:** Vector operations, transformations
- **Lighting Models:** Phong shading implementation
- **Performance Optimization:** Multi-threading and algorithmic efficiency
- **Software Architecture:** Object-oriented design patterns

## 📋 Assignment Requirements

- ✅ **C++11 Implementation:** Modern C++ standards
- ✅ **XML Scene Parsing:** Flexible scene description
- ✅ **Ray-Triangle Intersection:** Core rendering algorithm
- ✅ **Phong Lighting:** Realistic shading model
- ✅ **Multi-threading:** Performance optimization
- ✅ **Shadow Calculation:** Proper shadow rendering
- ✅ **Multiple Materials/Lights:** Scene complexity support
- ✅ **Texture Mapping:** Advanced material features

## 🤝 Contributing

This is an academic assignment. Please maintain academic integrity:
- Don't copy code directly
- Understand every implementation detail
- Be prepared to explain your solution approach

## 📄 Academic Context

**Course:** CSE 461 - Computer Graphics  
**Assignment:** Programming Assignment 1  
**Due Date:** April 8, 2025, 23:55  
**Type:** Individual Assignment

## 👨‍💻 Author

**Kurtburakk**
- GitHub: [@Kurtburakk](https://github.com/Kurtburakk)
- Course: CSE 461 - Computer Graphics

## 🙏 Acknowledgments

- Course instructor and TAs for project guidance
- Ray tracing algorithms from computer graphics literature
- XML parsing and image processing library communities
- Academic resources on photorealistic rendering techniques
