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

/* Global cube state variables */
float cubeWidth = DEFAULT_CUBE_WIDTH;
float cubeIncrementStep = CUBE_INCREMENT_STEP;
Rotation3D rotationAngles = {0.0f, 0.0f, 0.0f};

void drawCube(Rotation3D rotation)
{
    /* Validate cube parameters */
    if (cubeWidth <= 0.0f || cubeIncrementStep <= 0.0f) {
        return; /* Invalid cube dimensions */
    }

    /* Generate surface points for all six faces */
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += cubeIncrementStep)
    {
        for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += cubeIncrementStep)
        {
            /* Front face (z = -cubeWidth) */
            calculateForSurface((Point3D){cubeX, cubeY, -cubeWidth}, 'A', rotation);
            
            /* Right face (x = cubeWidth) */
            calculateForSurface((Point3D){cubeWidth, cubeY, cubeX}, 'B', rotation);
            
            /* Left face (x = -cubeWidth) */
            calculateForSurface((Point3D){-cubeWidth, cubeY, -cubeX}, 'C', rotation);
            
            /* Back face (z = cubeWidth) */
            calculateForSurface((Point3D){-cubeX, cubeY, cubeWidth}, 'D', rotation);
            
            /* Bottom face (y = -cubeWidth) */
            calculateForSurface((Point3D){cubeX, -cubeWidth, -cubeY}, 'E', rotation);
            
            /* Top face (y = cubeWidth) */
            calculateForSurface((Point3D){cubeX, cubeWidth, cubeY}, 'F', rotation);
        }
    }
}

void incrementRotationAngles(void)
{
    rotationAngles.x += ROTATION_INCREMENT;
    rotationAngles.y += ROTATION_INCREMENT;
}