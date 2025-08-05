/**
 * @file renderer.c
 * @author D. Heger
 * @brief Dynamic rendering and buffer management implementation
 * @version 1.1.0
 * @date 2025-08-05
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "renderer.h"
#include "math3d.h"

/* Global rendering state variables */
int windowWidth = DEFAULT_WINDOW_WIDTH;
int windowHeight = DEFAULT_WINDOW_HEIGHT;
int bufferSize = 0;
float* zBuffer = NULL;
char* frameBuffer = NULL;
int backgroundChar = BACKGROUND_CHARACTER;
int viewerDistance = DEFAULT_VIEWER_DISTANCE;
float projectionScaleFactor = DEFAULT_PROJECTION_SCALE;
int bufferIndex = 0;

/**
 * @brief Validate window dimensions against supported limits
 */
static int validateDimensions(int width, int height)
{
    return (width >= MIN_WINDOW_WIDTH && width <= MAX_WINDOW_WIDTH &&
            height >= MIN_WINDOW_HEIGHT && height <= MAX_WINDOW_HEIGHT);
}

int initializeRenderer(int width, int height)
{
    /* Validate input dimensions */
    if (!validateDimensions(width, height)) {
        return ALLOCATION_FAILURE;
    }

    /* Free existing buffers if allocated */
    cleanupRenderer();

    /* Set new dimensions */
    windowWidth = width;
    windowHeight = height;
    bufferSize = width * height;

    /* Allocate depth buffer */
    zBuffer = (float*)calloc(bufferSize, sizeof(float));
    if (zBuffer == NULL) {
        return ALLOCATION_FAILURE;
    }

    /* Allocate frame buffer */
    frameBuffer = (char*)malloc(bufferSize * sizeof(char));
    if (frameBuffer == NULL) {
        free(zBuffer);
        zBuffer = NULL;
        return ALLOCATION_FAILURE;
    }

    /* Initialize frame buffer with background character */
    memset(frameBuffer, backgroundChar, bufferSize);

    return ALLOCATION_SUCCESS;
}

void cleanupRenderer(void)
{
    if (zBuffer != NULL) {
        free(zBuffer);
        zBuffer = NULL;
    }
    
    if (frameBuffer != NULL) {
        free(frameBuffer);
        frameBuffer = NULL;
    }
    
    bufferSize = 0;
}

int resizeRenderer(int newWidth, int newHeight)
{
    /* Validate new dimensions */
    if (!validateDimensions(newWidth, newHeight)) {
        return ALLOCATION_FAILURE;
    }

    /* If dimensions haven't changed, nothing to do */
    if (newWidth == windowWidth && newHeight == windowHeight) {
        return ALLOCATION_SUCCESS;
    }

    /* Store old buffers for cleanup */
    float* oldZBuffer = zBuffer;
    char* oldFrameBuffer = frameBuffer;

    /* Try to allocate new buffers */
    int newBufferSize = newWidth * newHeight;
    float* newZBuffer = (float*)calloc(newBufferSize, sizeof(float));
    char* newFrameBuffer = (char*)malloc(newBufferSize * sizeof(char));

    if (newZBuffer == NULL || newFrameBuffer == NULL) {
        /* Allocation failed, cleanup and keep old buffers */
        free(newZBuffer);
        free(newFrameBuffer);
        return ALLOCATION_FAILURE;
    }

    /* Update global state */
    windowWidth = newWidth;
    windowHeight = newHeight;
    bufferSize = newBufferSize;
    zBuffer = newZBuffer;
    frameBuffer = newFrameBuffer;

    /* Initialize new frame buffer */
    memset(frameBuffer, backgroundChar, bufferSize);

    /* Free old buffers */
    free(oldZBuffer);
    free(oldFrameBuffer);

    return ALLOCATION_SUCCESS;
}

int initializeFrameBuffer(void)
{
    if (frameBuffer == NULL || bufferSize <= 0) {
        return ALLOCATION_FAILURE;
    }
    
    memset(frameBuffer, backgroundChar, bufferSize);
    return ALLOCATION_SUCCESS;
}

int initializeDepthBuffer(void)
{
    if (zBuffer == NULL || bufferSize <= 0) {
        return ALLOCATION_FAILURE;
    }
    
    memset(zBuffer, 0, bufferSize * sizeof(float));
    return ALLOCATION_SUCCESS;
}

void calculateForSurface(Point3D cubePoint, int ch, Rotation3D rotation)
{
    /* Verify buffers are allocated */
    if (frameBuffer == NULL || zBuffer == NULL || bufferSize <= 0) {
        return; /* Buffers not properly initialized */
    }

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

    /* Comprehensive bounds checking for screen coordinates */
    if (screenX < 0 || screenX >= windowWidth || screenY < 0 || screenY >= windowHeight) {
        return; /* Point is outside screen boundaries */
    }

    /* Calculate buffer index with comprehensive bounds verification */
    bufferIndex = screenX + screenY * windowWidth;
    
    /* Double-check buffer index bounds */
    if (bufferIndex < 0 || bufferIndex >= bufferSize) {
        return; /* Buffer index out of bounds */
    }

    /* Verify buffer validity before access */
    if (frameBuffer == NULL || zBuffer == NULL) {
        return; /* Buffers became invalid */
    }

    /* Z-buffer test for visibility with bounds-safe access */
    if (inverseDepth > zBuffer[bufferIndex]) {
        zBuffer[bufferIndex] = inverseDepth;
        frameBuffer[bufferIndex] = (char)ch;
    }
}