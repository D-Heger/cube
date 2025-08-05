/**
 * @file main.c
 * @author D. Heger
 * @brief Main program loop for 3D Cube
 *
 * This project is designed to demonstrate the manipulation of 3D objects,
 * specifically a cube, using C programming.
 * The modular design separates concerns into math3d for 3D calculations,
 * renderer for display management, input for user interaction, and cube
 * for cube-specific logic.
 *
 * @version 1.1.0
 * @date 2025-08-05
 */

#include <stdio.h>
#include "math3d.h"
#include "renderer.h"
#include "input.h"
#include "cube.h"

/**
 * @brief Main program entry point.
 *
 * This function initializes the program, sets up terminal for non-blocking input,
 * and runs the main animation loop. The loop continuously:
 * 1. Initializes frame and depth buffers
 * 2. Draws the cube with current rotation
 * 3. Renders the frame to terminal
 * 4. Checks for user input to exit
 * 5. Updates rotation angles for animation
 * 6. Waits before next frame
 *
 * The program exits when the user presses '1' key.
 *
 * @return int Program exit status (0 for success).
 */
int main()
{
    printf("\x1b[2J"); /* Clear the terminal screen by sending the terminal control sequence for clear screen. */

    setNonBlockingMode(); /* Ensure terminal is set to non-blocking mode. */

    while (1) /* Start an infinite loop to continuously update and display the cube's rotation. */
    {
        initializeFrameBuffer(); /* Initialize the frame buffer with the background character. */
        initializeDepthBuffer(); /* Initialize the depth buffer with zeros to track the depth of each pixel. */

        drawCube(rotationAngles); /* Draw the cube in its current rotation state to the frame and depth buffers. */

        printf("\x1b[H"); /* Move the cursor to the home position (top-left corner of the terminal) for drawing. */

        /*
         * Iterate over each character in the frame buffer and print it.
         * This effectively draws the entire frame buffer to the terminal screen.
         */
        for (int k = 0; k < windowWidth * windowHeight; k++)
        {
            /*
             * Print each character. If at the end of a row (not divisible by windowWidth), print the character.
             * Otherwise, insert a newline to move to the next row.
             */
            putchar(k % windowWidth ? frameBuffer[k] : '\n');
        }

        if (isKeyPressed())
        {          /* Check if '1' is pressed */
            break; /* Exit the loop if '1' is pressed */
        }
        incrementRotationAngles(); /* Increment the rotation angles for the next frame to animate the cube. */
        wait(1000);                /* Wait for a short period (1000 microseconds) before drawing the next frame. */
    }

    resetTerminalMode(); /* Reset terminal to original settings before exiting. */

    return 0;
}