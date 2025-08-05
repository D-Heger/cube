/**
 * @file cube.h
 * @author D. Heger
 * @brief Cube geometry, animation, and rendering logic
 * @version 1.1.0
 * @date 2025-08-05
 */

#ifndef CUBE_H
#define CUBE_H

#include "math3d.h"
#include "constants.h"

/**
 * @brief Current cube size (width/height/depth)
 * 
 * Defines the dimensions of the cube in world coordinates. The cube
 * extends from -cubeWidth to +cubeWidth in all three axes, creating
 * a cube with total dimension of 2*cubeWidth per side.
 */
extern float cubeWidth;

/**
 * @brief Surface sampling resolution step size
 * 
 * Controls the density of points generated on each cube face. Smaller
 * values create more detailed surfaces but increase rendering cost.
 * Must be positive and less than cubeWidth for proper coverage.
 */
extern float cubeIncrementStep;

/**
 * @brief Current animation rotation state
 * 
 * Tracks the current rotation angles for animated cube display.
 * Values are continuously updated by incrementRotationAngles() to
 * create smooth rotation animation.
 */
extern Rotation3D rotationAngles;

/**
 * @brief Render all six faces of the cube with rotation applied
 *
 * Generates surface points for all six cube faces and renders them through
 * the projection pipeline. Each face is represented by a different character:
 * - 'A': Front face (z = -cubeWidth)
 * - 'B': Right face (x = +cubeWidth)  
 * - 'C': Left face (x = -cubeWidth)
 * - 'D': Back face (z = +cubeWidth)
 * - 'E': Bottom face (y = -cubeWidth)
 * - 'F': Top face (y = +cubeWidth)
 *
 * @param rotation The 3D rotation angles to apply to all cube points (in radians)
 * 
 * @note Calls calculateForSurface() for each generated point
 * @note Surface density controlled by cubeIncrementStep value
 * @note Does not perform bounds checking on rotation parameters
 */
void drawCube(Rotation3D rotation);

/**
 * @brief Update rotation angles for animation
 *
 * Advances the global rotation state by fixed increments to create
 * smooth animated rotation. Updates both X and Y rotation angles
 * simultaneously for diagonal spinning effect.
 *
 * @note Modifies the global rotationAngles variable
 * @note Rotation increment is defined by ROTATION_INCREMENT constant
 * @note No bounds checking - angles will continue increasing indefinitely
 */
void incrementRotationAngles(void);

#endif // CUBE_H