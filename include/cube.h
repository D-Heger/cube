/**
 * @file cube.h
 * @author D. Heger
 * @brief Cube-specific logic declarations
 * @version 1.1.0
 * @date 2025-08-05
 */

#ifndef CUBE_H
#define CUBE_H

#include "math3d.h"

/**
 * @brief The width of the cube.
 */
extern float cubeWidth;

/**
 * @brief The step size for incrementing the cube.
 */
extern float cubeIncrementStep;

/**
 * @brief The current rotation angles of the cube.
 */
extern Rotation3D rotationAngles;

/**
 * @brief Draws the cube with the specified rotation.
 *
 * This function iterates through all points on the cube's six faces
 * and renders each visible surface point. It generates points across
 * each face of the cube and calls the surface calculation function
 * to project and display them with appropriate characters.
 *
 * @param rotation The rotation of the cube to be applied during rendering.
 */
void drawCube(Rotation3D rotation);

/**
 * @brief Increments the rotation angles.
 *
 * This function updates the cube's rotation angles for animation.
 * It incrementally increases both X and Y rotation angles to create
 * a continuous spinning effect for the cube display.
 */
void incrementRotationAngles();

#endif // CUBE_H