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
#include "constants.h"

/**
 * @brief The width and height of the display window
 * 
 * These values define the terminal display dimensions in characters.
 * They are initialized to default values but can be modified at runtime.
 */
extern int windowWidth, windowHeight;

/**
 * @brief Depth buffer for Z-buffering and visibility testing
 * 
 * Stores the inverse depth (1/z) value for each pixel to determine
 * which surfaces are visible. Higher values indicate closer objects.
 */
extern float zBuffer[BUFFER_SIZE];

/**
 * @brief Frame buffer containing the characters to display
 * 
 * Each element corresponds to one character position on the terminal screen.
 * Characters represent different cube faces or background space.
 */
extern char frameBuffer[BUFFER_SIZE];

/**
 * @brief Background character used for empty screen areas
 * 
 * ASCII character code used to fill background/empty pixels in the frame buffer.
 * Typically set to space character (32) for clear background.
 */
extern int backgroundChar;

/**
 * @brief Distance from the viewer/camera to the cube center
 * 
 * Controls the perspective projection. Larger values create less perspective distortion,
 * smaller values create more dramatic perspective effects.
 */
extern int viewerDistance;

/**
 * @brief Scale factor for perspective projection calculations
 * 
 * Determines how large the projected cube appears on screen. Higher values
 * result in larger cube visualization.
 */
extern float projectionScaleFactor;

/**
 * @brief Current buffer index for pixel operations
 * 
 * Temporary variable used during rendering to track the current position
 * in the frame and depth buffers. Should not be accessed directly.
 */
extern int bufferIndex;

/**
 * @brief Initialize the frame buffer for a new frame
 * 
 * Clears the frame buffer by filling all positions with the background character.
 * Must be called before each frame to ensure clean rendering.
 * 
 * @note This function does not perform error checking on buffer access
 */
void initializeFrameBuffer(void);

/**
 * @brief Initialize the depth buffer for Z-buffer testing
 * 
 * Resets all depth buffer values to zero, preparing for depth testing during rendering.
 * Must be called before each frame to ensure proper visibility calculations.
 * 
 * @note This function does not perform error checking on buffer access
 */
void initializeDepthBuffer(void);

/**
 * @brief Project and render a 3D point to the screen buffer
 * 
 * Applies 3D rotation transformations to the input point, projects it to 2D screen
 * coordinates using perspective projection, and updates the frame buffer if the point
 * passes depth testing. Uses Z-buffering to handle surface visibility correctly.
 * 
 * The projection formula used is:
 * - screenX = windowWidth/2 + (projectionScaleFactor * x * 2) / z
 * - screenY = windowHeight/2 + (projectionScaleFactor * y) / z
 * 
 * @param cubePoint The 3D point on the cube surface to project and render
 * @param ch The ASCII character to draw if this point is visible (typically 'A'-'F' for cube faces)
 * @param rotation The 3D rotation angles (in radians) to apply before projection
 * 
 * @note Points outside screen boundaries are safely ignored
 * @note Uses inverse depth (1/z) for depth comparison to avoid division by zero
 */
void calculateForSurface(Point3D cubePoint, int ch, Rotation3D rotation);

#endif // RENDERER_H