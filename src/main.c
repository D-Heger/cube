/**
 * @file main.c
 * @author D. Heger
 * @brief Main program loop for 3D Cube visualization
 *
 * This project demonstrates 3D object manipulation using modular C programming.
 * The architecture separates concerns across specialized modules:
 * - math3d: 3D transformation mathematics
 * - renderer: Display buffer management and projection
 * - input: Terminal interaction and timing
 * - cube: Geometry generation and animation
 *
 * @version 1.1.0
 * @date 2025-08-05
 */

#include <stdio.h>
#include "constants.h"
#include "math3d.h"
#include "renderer.h"
#include "input.h"
#include "cube.h"

/* External buffer size reference */
extern int bufferSize;
extern char* frameBuffer;

int main(void)
{
    /* Initialize rendering system with default dimensions */
    if (initializeRenderer(DEFAULT_WINDOW_WIDTH, DEFAULT_WINDOW_HEIGHT) != ALLOCATION_SUCCESS) {
        fprintf(stderr, "Error: Failed to initialize rendering system\n");
        return 1;
    }

    /* Initialize terminal display */
    printf(CLEAR_SCREEN_SEQ);
    setNonBlockingMode();

    /* Main animation loop */
    while (1) {
        /* Clear buffers for new frame */
        if (initializeFrameBuffer() != ALLOCATION_SUCCESS ||
            initializeDepthBuffer() != ALLOCATION_SUCCESS) {
            fprintf(stderr, "Error: Buffer initialization failed\n");
            break;
        }

        /* Generate and render cube geometry */
        drawCube(rotationAngles);

        /* Display frame buffer to terminal */
        printf(HOME_CURSOR_SEQ);
        
        /* Bounds-safe buffer display */
        if (frameBuffer != NULL && bufferSize > 0) {
            for (int pixelIndex = 0; pixelIndex < bufferSize; pixelIndex++) {
                putchar(pixelIndex % windowWidth ? frameBuffer[pixelIndex] : '\n');
            }
        }

        /* Check for exit condition */
        if (isKeyPressed()) {
            break;
        }

        /* Update animation state */
        incrementRotationAngles();
        wait(FRAME_DELAY_MICROSEC);
    }

    /* Cleanup and exit */
    resetTerminalMode();
    cleanupRenderer();
    return 0;
}