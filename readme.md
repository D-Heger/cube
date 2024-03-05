
# Cube Project

## Description
This project is designed to demonstrate the manipulation of 3D objects, specifically a cube, using C programming. The `cube.c` file contains the implementation for 3D point and rotation structures, among other functionalities. This simple yet powerful example serves as a basis for understanding 3D geometry manipulation in computer graphics.

## Features
- Definition and manipulation of 3D points.
- 3D rotation structures and algorithms.
- Simple CLI interface for interacting with the cube model.

## Getting Started
To get started with the Cube project, clone this repository and build the project using the provided Makefile.

### Prerequisites
- GCC compiler
- Basic C library

### Build Instructions
```bash
make build
```

This command compiles the `cube.c` file into an executable named `cube`. It links the necessary libraries, including math (`-lm`), to ensure the program runs correctly.

### Running the Program
After building, you can run the program using:
```bash
make run
```

Alternately, you also can both build and run the program by using:
```bash
make all
```

## Changelog
### v1.0.0
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
