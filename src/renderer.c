/**
 * @file renderer.c
 * @author D. Heger
 * @brief Rendering and buffer management implementation
 * @version 1.1.0
 * @date 2025-08-05
 */

#include <string.h>
#include <stdio.h>
#include "renderer.h"
#include "math3d.h"

/**
 * @brief The width and height of the window.
 */
int windowWidth = 80, windowHeight = 22;

/**
 * @brief A depth buffer to store the depth of each pixel.
 */
float zBuffer[80 * 22];

/**
 * @brief A frame buffer to store the characters to be displayed on the screen.
 */
char frameBuffer[80 * 22];

/**
 * @brief The character to be used for the background.
 */
int backgroundChar = ' ';

/**
 * @brief The distance of the viewer from the cube.
 */
int viewerDistance = 60;

/**
 * @brief The scale factor for the projection matrix.
 */
float projectionScaleFactor = 40;

/**
 * @brief The index of the current position in the frame buffer.
 */
int bufferIndex;

/**
 * @brief Initializes the frame buffer.
 *
 * Fills the frame buffer with the background character to prepare for rendering.
 * This clears any previous frame data and sets up a clean canvas for the next frame.
 */
void initializeFrameBuffer()
{
    memset(frameBuffer, backgroundChar, windowWidth * windowHeight);
}

/**
 * @brief Initializes the depth buffer.
 *
 * Resets all depth buffer values to zero to prepare for depth testing.
 * This ensures that depth comparisons start fresh for each frame.
 */
void initializeDepthBuffer()
{
    memset(zBuffer, 0, windowWidth * windowHeight * sizeof(float));
}

/**
 * @brief Projects a point on the cube's surface to screen coordinates and updates the frame buffer if the point is visible.
 *
 * This function takes a point on the surface of a cube, applies a 3D rotation to it, and then projects it onto a 2D screen space.
 * It calculates the screen coordinates based on the perspective projection formula, taking into account the viewer's distance.
 * If the calculated screen position is within the display boundaries and the point is closer to the viewer than any previously
 * rendered point at the same position, it updates the zBuffer and frameBuffer to include the character representation of the surface.
 *
 * The projection uses perspective transformation to convert 3D coordinates to 2D screen coordinates.
 * Z-buffering is employed to handle depth testing and ensure proper visibility of surfaces.
 *
 * @param cubePoint The point on the cube's surface to be projected.
 * @param ch The character to draw at the projected point's screen coordinates if it is visible.
 * @param rotation The current rotation of the cube, to be applied to the cubePoint before projecting it.
 */
void calculateForSurface(Point3D cubePoint, int ch, Rotation3D rotation)
{
    Point3D projected;
    projected.x = calculateX(cubePoint, rotation);
    projected.y = calculateY(cubePoint, rotation);
    projected.z = calculateZ(cubePoint, rotation) + viewerDistance;

    float inverseDepth = 1 / projected.z;

    int screenX = (int)(windowWidth / 2 + projectionScaleFactor * inverseDepth * projected.x * 2);
    int screenY = (int)(windowHeight / 2 + projectionScaleFactor * inverseDepth * projected.y);

    bufferIndex = screenX + screenY * windowWidth;
    if (bufferIndex >= 0 && bufferIndex < windowWidth * windowHeight)
    {
        if (inverseDepth > zBuffer[bufferIndex])
        {
            zBuffer[bufferIndex] = inverseDepth;
            frameBuffer[bufferIndex] = ch;
        }
    }
}