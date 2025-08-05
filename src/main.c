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

int main(void)
{
    /* Initialize terminal display */
    printf(CLEAR_SCREEN_SEQ);
    setNonBlockingMode();

    /* Main animation loop */
    while (1) {
        /* Clear buffers for new frame */
        initializeFrameBuffer();
        initializeDepthBuffer();

        /* Generate and render cube geometry */
        drawCube(rotationAngles);

        /* Display frame buffer to terminal */
        printf(HOME_CURSOR_SEQ);
        for (int pixelIndex = 0; pixelIndex < BUFFER_SIZE; pixelIndex++) {
            putchar(pixelIndex % windowWidth ? frameBuffer[pixelIndex] : '\n');
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
    return 0;
}