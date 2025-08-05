/**
 * @file cube.c
 * @author D. Heger
 * @brief Cube-specific logic implementation
 * @version 1.1.0
 * @date 2025-08-05
 */

#include "cube.h"
#include "renderer.h"
#include "math3d.h"

/**
 * @brief The width of the cube.
 */
float cubeWidth = 10;

/**
 * @brief The step size for incrementing the cube.
 */
float cubeIncrementStep = 1;

/**
 * @brief The current rotation angles of the cube.
 */
Rotation3D rotationAngles = {0, 0, 0}; /* Set initial values */

/**
 * @brief Draws the cube with the specified rotation.
 *
 * This function iterates through all points on the cube's six faces
 * and renders each visible surface point. It generates points across
 * each face of the cube and calls the surface calculation function
 * to project and display them with appropriate characters.
 *
 * Each face of the cube is represented by a different character:
 * - 'A': Front face (z = -cubeWidth)
 * - 'B': Right face (x = cubeWidth)
 * - 'C': Left face (x = -cubeWidth)
 * - 'D': Back face (z = cubeWidth)
 * - 'E': Bottom face (y = -cubeWidth)
 * - 'F': Top face (y = cubeWidth)
 *
 * @param rotation The rotation of the cube to be applied during rendering.
 */
void drawCube(Rotation3D rotation)
{
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += cubeIncrementStep)
    {
        for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += cubeIncrementStep)
        {
            calculateForSurface((Point3D){cubeX, cubeY, -cubeWidth}, 'A', rotation);
            calculateForSurface((Point3D){cubeWidth, cubeY, cubeX}, 'B', rotation);
            calculateForSurface((Point3D){-cubeWidth, cubeY, -cubeX}, 'C', rotation);
            calculateForSurface((Point3D){-cubeX, cubeY, cubeWidth}, 'D', rotation);
            calculateForSurface((Point3D){cubeX, -cubeWidth, -cubeY}, 'E', rotation);
            calculateForSurface((Point3D){cubeX, cubeWidth, cubeY}, 'F', rotation);
        }
    }
}

/**
 * @brief Increments the rotation angles.
 *
 * This function updates the cube's rotation angles for animation.
 * It incrementally increases both X and Y rotation angles to create
 * a continuous spinning effect for the cube display. The increment
 * values are chosen to provide smooth animation at reasonable speed.
 */
void incrementRotationAngles()
{
    rotationAngles.x += 0.005;
    rotationAngles.y += 0.005;
}