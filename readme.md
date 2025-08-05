
# 3D Cube Visualization Project

## Description

This project demonstrates 3D object manipulation and visualization using C programming. The modular design showcases professional software engineering practices with clean separation of concerns across multiple modules for 3D mathematics, rendering, input handling, and cube-specific logic.

## Project Structure

```txt
cube/
├── src/                  # Source files
│   ├── main.c            # Main program loop and initialization
│   ├── math3d.c          # 3D mathematics and rotation calculations
│   ├── renderer.c        # Rendering engine and buffer management
│   ├── input.c           # Terminal input handling and controls
│   └── cube.c            # Cube-specific drawing and animation logic
├── include/              # Header files
│   ├── math3d.h          # 3D structures and function declarations
│   ├── renderer.h        # Rendering system declarations
│   ├── input.h           # Input handling declarations
│   └── cube.h            # Cube logic declarations
├── build/                # Build artifacts (created during compilation)
├── Makefile              # Build system configuration
└── readme.md             # Project documentation
```

## Features

- **Modular Architecture**: Clean separation into math3d, renderer, input, and cube modules
- **3D Mathematics**: Complete 3D point and rotation transformation system
- **Real-time Rendering**: Frame buffer and Z-buffer based 3D rendering
- **Interactive Controls**: Non-blocking terminal input with '1' key to exit
- **Professional Build System**: Organized directory structure with automated dependency management

## Module Overview

### math3d Module

- **Point3D** and **Rotation3D** structures for 3D geometry
- Rotation transformation functions for X, Y, Z coordinates
- Mathematical foundation for 3D object manipulation

### renderer Module

- Frame buffer and depth buffer management
- Perspective projection and screen coordinate mapping
- Z-buffering for proper depth testing and visibility

### input Module

- Non-blocking terminal input detection
- Terminal mode management (canonical/non-canonical)
- Timing and delay functions for animation control

### cube Module

- Cube geometry generation and face mapping
- Animation rotation angle management  
- Integration with rendering system for visualization

## Getting Started

### Prerequisites

- GCC compiler with GNU99 standard support
- POSIX-compatible system (Linux/macOS/WSL)
- Make build system

### Build Instructions

```bash
# Build the project
make

# Or explicitly build without running
make cube

# Clean build artifacts
make clean

# Rebuild from scratch
make rebuild
```

### Running the Program

```bash
# Run after building
make run

# Or run the executable directly
./cube
```

### Controls

- **'1' key**: Exit the program
- The cube rotates automatically with smooth animation
- Press Ctrl+C as fallback exit method

## Changelog

### v1.1.0 - 2025-08-05

- Modularized codebase: Improved organization by separating source files into distinct modules for better maintainability and clarity.

### v1.0.0 - 2024-03-05

- Initial release of the Cube project: This foundational release sets the stage for 3D geometric manipulations, specifically focusing on cube interactions.
- Implementation of 3D point and rotation structures: Introduced data structures for representing points and rotations in a 3D space, laying the groundwork for geometric calculations and transformations.
- Advanced rendering techniques:
  - drawCube function: Implements the core functionality for drawing the cube based on current rotation angles and positions.
  - calculateForSurface function: A specialized function for computing surface visibility and effects, enhancing the cube's visual representation.
- Interactive rotation and visualization:
  - incrementRotationAngles function: Allows dynamic adjustment of the cube's rotation angles, offering interactive visualization experiences.
- Enhanced terminal interaction:
  - setNonBlockingMode and resetTerminalMode functions: Ensure a smooth user interaction within the terminal, optimizing the program's usability across different terminal environments.
  - Performance optimizations:
  - initializeFrameBuffer and initializeDepthBuffer functions: These functions are integral to optimizing the rendering process, ensuring efficient frame drawing and depth calculations.
- User experience improvements:
  - wait function: Implements a simple delay mechanism, enhancing the flow of visualization and interaction by moderating the speed of cube rotations and updates.
  - Build and run instructions provided in Makefile: Streamlined compilation and execution process, ensuring users can easily build and run the Cube project with minimal setup.

## Authors

- D. Heger

## License

This project is open-source and available under the [MIT License](LICENSE).
