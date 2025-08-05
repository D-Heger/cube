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
 * @brief Current display window dimensions
 * 
 * These values define the terminal display dimensions in characters.
 * They can be dynamically configured within MIN/MAX limits.
 */
extern int windowWidth, windowHeight;

/**
 * @brief Current buffer size based on window dimensions
 * 
 * Calculated as windowWidth * windowHeight. Updated when window size changes.
 */
extern int bufferSize;

/**
 * @brief Dynamically allocated depth buffer for Z-buffering
 * 
 * Stores the inverse depth (1/z) value for each pixel to determine
 * which surfaces are visible. Allocated based on current window size.
 */
extern float* zBuffer;

/**
 * @brief Dynamically allocated frame buffer for display characters
 * 
 * Each element corresponds to one character position on the terminal screen.
 * Allocated based on current window size.
 */
extern char* frameBuffer;

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
 * @brief Initialize the rendering system with specified window dimensions
 * 
 * Allocates memory for frame and depth buffers based on the specified dimensions.
 * Must be called before any rendering operations.
 * 
 * @param width Display width in characters (must be within MIN/MAX limits)
 * @param height Display height in characters (must be within MIN/MAX limits)
 * @return ALLOCATION_SUCCESS on success, ALLOCATION_FAILURE on error
 * 
 * @note Validates input dimensions against MIN/MAX constants
 * @note Frees any existing buffers before allocating new ones
 */
int initializeRenderer(int width, int height);

/**
 * @brief Cleanup and free all allocated rendering resources
 * 
 * Deallocates frame and depth buffers. Should be called before program exit
 * or when changing window dimensions.
 * 
 * @note Safe to call multiple times or with NULL buffers
 */
void cleanupRenderer(void);

/**
 * @brief Resize the rendering buffers to new dimensions
 * 
 * Reallocates buffers with new dimensions, preserving existing content where possible.
 * 
 * @param newWidth New display width in characters
 * @param newHeight New display height in characters
 * @return ALLOCATION_SUCCESS on success, ALLOCATION_FAILURE on error
 * 
 * @note Validates new dimensions before allocation
 * @note Falls back to previous size on allocation failure
 */
int resizeRenderer(int newWidth, int newHeight);

/**
 * @brief Initialize the frame buffer for a new frame
 * 
 * Clears the frame buffer by filling all positions with the background character.
 * Must be called before each frame to ensure clean rendering.
 * 
 * @return ALLOCATION_SUCCESS on success, ALLOCATION_FAILURE if buffer not allocated
 */
int initializeFrameBuffer(void);

/**
 * @brief Initialize the depth buffer for Z-buffer testing
 * 
 * Resets all depth buffer values to zero, preparing for depth testing during rendering.
 * Must be called before each frame to ensure proper visibility calculations.
 * 
 * @return ALLOCATION_SUCCESS on success, ALLOCATION_FAILURE if buffer not allocated
 */
int initializeDepthBuffer(void);

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