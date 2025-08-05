/**
 * @file renderer.h
 * @author D. Heger
 * @brief Rendering and buffer management declarations
 * @version 1.1.0
 * @date 2025-08-05
 */

#ifndef RENDERER_H
#define RENDERER_H

#include "math3d.h"

/**
 * @brief The width and height of the window.
 */
extern int windowWidth, windowHeight;

/**
 * @brief A depth buffer to store the depth of each pixel.
 */
extern float zBuffer[80 * 22];

/**
 * @brief A frame buffer to store the characters to be displayed on the screen.
 */
extern char frameBuffer[80 * 22];

/**
 * @brief The character to be used for the background.
 */
extern int backgroundChar;

/**
 * @brief The distance of the viewer from the cube.
 */
extern int viewerDistance;

/**
 * @brief The scale factor for the projection matrix.
 */
extern float projectionScaleFactor;

/**
 * @brief The index of the current position in the frame buffer.
 */
extern int bufferIndex;

/**
 * @brief Initializes the frame buffer.
 *
 * Fills the frame buffer with the background character to prepare for rendering.
 */
void initializeFrameBuffer();

/**
 * @brief Initializes the depth buffer.
 *
 * Resets all depth buffer values to zero to prepare for depth testing.
 */
void initializeDepthBuffer();

/**
 * @brief Projects a point on the cube's surface to screen coordinates and updates the frame buffer if the point is visible.
 *
 * This function takes a point on the surface of a cube, applies a 3D rotation to it, and then projects it onto a 2D screen space.
 * It calculates the screen coordinates based on the perspective projection formula, taking into account the viewer's distance.
 * If the calculated screen position is within the display boundaries and the point is closer to the viewer than any previously
 * rendered point at the same position, it updates the zBuffer and frameBuffer to include the character representation of the surface.
 *
 * @param cubePoint The point on the cube's surface to be projected.
 * @param ch The character to draw at the projected point's screen coordinates if it is visible.
 * @param rotation The current rotation of the cube, to be applied to the cubePoint before projecting it.
 */
void calculateForSurface(Point3D cubePoint, int ch, Rotation3D rotation);

#endif // RENDERER_H