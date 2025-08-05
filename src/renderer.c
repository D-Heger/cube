/**
 * @file renderer.c
 * @author D. Heger
 * @brief Rendering and buffer management implementation
 * @version 1.1.0
 * @date 2025-08-05
 */

#include <string.h>
#include <stdio.h>
#include <math.h>
#include "renderer.h"
#include "math3d.h"

/* Global rendering state variables */
int windowWidth = WINDOW_WIDTH;
int windowHeight = WINDOW_HEIGHT;
float zBuffer[BUFFER_SIZE];
char frameBuffer[BUFFER_SIZE];
int backgroundChar = BACKGROUND_CHARACTER;
int viewerDistance = DEFAULT_VIEWER_DISTANCE;
float projectionScaleFactor = DEFAULT_PROJECTION_SCALE;

int bufferIndex; /* Current buffer position index */

void initializeFrameBuffer(void)
{
    memset(frameBuffer, backgroundChar, BUFFER_SIZE);
}

void initializeDepthBuffer(void)
{
    memset(zBuffer, 0, BUFFER_SIZE * sizeof(float));
}

void calculateForSurface(Point3D cubePoint, int ch, Rotation3D rotation)
{
    /* Apply 3D rotation transformations */
    Point3D projected;
    projected.x = calculateX(cubePoint, rotation);
    projected.y = calculateY(cubePoint, rotation);
    projected.z = calculateZ(cubePoint, rotation) + viewerDistance;

    /* Guard against division by zero or negative depth */
    if (projected.z <= 0.0f) {
        return; /* Point is behind viewer or at viewer position */
    }

    /* Calculate perspective projection */
    float inverseDepth = 1.0f / projected.z;
    int screenX = (int)(windowWidth / 2 + projectionScaleFactor * inverseDepth * projected.x * 2);
    int screenY = (int)(windowHeight / 2 + projectionScaleFactor * inverseDepth * projected.y);

    /* Bounds checking for screen coordinates */
    if (screenX < 0 || screenX >= windowWidth || screenY < 0 || screenY >= windowHeight) {
        return; /* Point is outside screen boundaries */
    }

    /* Calculate buffer index with bounds verification */
    bufferIndex = screenX + screenY * windowWidth;
    if (bufferIndex >= 0 && bufferIndex < BUFFER_SIZE) {
        /* Z-buffer test for visibility */
        if (inverseDepth > zBuffer[bufferIndex]) {
            zBuffer[bufferIndex] = inverseDepth;
            frameBuffer[bufferIndex] = (char)ch;
        }
    }
}