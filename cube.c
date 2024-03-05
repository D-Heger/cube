/**
 * @file main.c
 * @author D. Heger
 * @brief A brief description of the program.
 *
 * A more detailed description of the program.
 *
 * @version 0.1
 * @date 2024-03-05
 *
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

/**
 * @brief A structure to represent a 3D point.
 *
 */
typedef struct
{
    float x, y, z;
} Point3D;

/**
 * @brief A structure to represent a 3D rotation.
 *
 */
typedef struct
{
    float x, y, z;
} Rotation3D;

/**
 * @brief The width of the cube.
 *
 */
float cubeWidth = 10;

/**
 * @brief The width and height of the window.
 *
 */
int windowWidth = 80, windowHeight = 22;

/**
 * @brief A depth buffer to store the depth of each pixel.
 *
 */
float zBuffer[80 * 22];

/**
 * @brief A frame buffer to store the characters to be displayed on the screen.
 *
 */
char frameBuffer[80 * 22];

/**
 * @brief The character to be used for the background.
 *
 */
int backgroundChar = ' ';

/**
 * @brief The step size for incrementing the cube.
 *
 */
float cubeIncrementStep = 1;

/**
 * @brief The distance of the viewer from the cube.
 *
 */
int viewerDistance = 60;

/**
 * @brief The scale factor for the projection matrix.
 *
 */
float projectionScaleFactor = 40;

/**
 * @brief The index of the current position in the frame buffer.
 *
 */
int bufferIndex;

/**
 * @brief The current rotation angles of the cube.
 *
 */
Rotation3D rotationAngles = {0, 0, 0}; // Set initial values

/**
 * @brief Calculates the X coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * @param point The point to calculate the X coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The X coordinate of the point.
 */
float calculateX(Point3D point, Rotation3D rotation)
{
    return point.y * sin(rotation.x) * sin(rotation.y) * cos(rotation.z) -
           point.z * cos(rotation.x) * sin(rotation.y) * cos(rotation.z) +
           point.y * cos(rotation.x) * sin(rotation.z) +
           point.z * sin(rotation.x) * sin(rotation.z) +
           point.x * cos(rotation.y) * cos(rotation.z);
}

/**
 * @brief Calculates the Y coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * @param point The point to calculate the Y coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The Y coordinate of the point.
 */
float calculateY(Point3D point, Rotation3D rotation)
{
    return point.y * cos(rotation.x) * cos(rotation.z) +
           point.z * sin(rotation.x) * cos(rotation.z) -
           point.y * sin(rotation.x) * sin(rotation.y) * sin(rotation.z) +
           point.z * cos(rotation.x) * sin(rotation.y) * sin(rotation.z) -
           point.x * cos(rotation.y) * sin(rotation.z);
}

/**
 * @brief Calculates the Z coordinate of a point in 3D space based on the rotation of the coordinate system.
 *
 * @param point The point to calculate the Z coordinate for.
 * @param rotation The rotation of the coordinate system.
 * @return The Z coordinate of the point.
 */
float calculateZ(Point3D point, Rotation3D rotation)
{
    return point.z * cos(rotation.x) * cos(rotation.y) -
           point.y * sin(rotation.x) * cos(rotation.y) +
           point.x * sin(rotation.y);
}

/**
 * @brief Initializes the frame buffer.
 *
 */
void initializeFrameBuffer()
{
    memset(frameBuffer, backgroundChar, windowWidth * windowHeight);
}

/**
 * @brief Initializes the depth buffer.
 *
 */
void initializeDepthBuffer()
{
    memset(zBuffer, 0, windowWidth * windowHeight * sizeof(float));
}

/**
 * @brief Draws the cube with the specified rotation.
 *
 * @param rotation The rotation of the cube.
 *
 */
void drawCube(Rotation3D rotation)
{
    for (float cubeX = -cubeWidth; cubeX < cubeWidth; cubeX += cubeIncrementStep)
    {
        for (float cubeY = -cubeWidth; cubeY < cubeWidth; cubeY += cubeIncrementStep)
        {
            calculateForSurface((Point3D){cubeX, cubeY, -cubeWidth}, 'A', rotation);
            calculateForSurface((Point3D){cubeWidth, cubeY, cubeX}, 'B', rotation);
            calculateForSurface((Point3D){-cubeWidth, cubeY, -cubeX}, 'C', rotation);
            calculateForSurface((Point3D){-cubeX, cubeY, cubeWidth}, 'D', rotation);
            calculateForSurface((Point3D){cubeX, -cubeWidth, -cubeY}, 'E', rotation);
            calculateForSurface((Point3D){cubeX, cubeWidth, cubeY}, 'F', rotation);
        }
    }
}

/**
 * @brief Increments the rotation angles.
 *
 */
void incrementRotationAngles()
{
    rotationAngles.x += 0.005;
    rotationAngles.y += 0.005;
}

/**
 * @brief Waits for the specified number of microseconds.
 *
 * @param sleep The number of microseconds to wait.
 *
 */
void wait(int sleep)
{
    usleep(sleep);
}

/**
 * @brief Set the terminal to non-blocking mode to detect key presses.
 *
 */
void setNonBlockingMode()
{
    struct termios ttystate;

    // get the terminal state
    tcgetattr(STDIN_FILENO, &ttystate);

    // turn off canonical mode and echo
    ttystate.c_lflag &= ~(ICANON | ECHO);
    // minimum of number input read.
    ttystate.c_cc[VMIN] = 1;

    // set the terminal attributes.
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

/**
 * @brief Reset the terminal to its original settings.
 *
 */
void resetTerminalMode()
{
    struct termios ttystate;
    tcgetattr(STDIN_FILENO, &ttystate);
    ttystate.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

/**
 * @brief Check if a specific key is pressed without blocking.
 *
 * @return int Returns 1 if the specific key is pressed, 0 otherwise.
 */
int isKeyPressed()
{
    // Non-blocking check for keyboard input
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Save old settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Disable buffering
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

    // Apply non-blocking read
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    // Restore old settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == '1')
    {
        ungetc(ch, stdin); // Put the character back, in case further handling is needed
        return 1;
    }

    return 0;
}

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

int main()
{
    printf("\x1b[2J"); // Clear the terminal screen by sending the terminal control sequence for clear screen.

    setNonBlockingMode(); // Ensure terminal is set to non-blocking mode.

    while (1) // Start an infinite loop to continuously update and display the cube's rotation.
    {
        initializeFrameBuffer(); // Initialize the frame buffer with the background character.
        initializeDepthBuffer(); // Initialize the depth buffer with zeros to track the depth of each pixel.

        drawCube(rotationAngles); // Draw the cube in its current rotation state to the frame and depth buffers.

        printf("\x1b[H"); // Move the cursor to the home position (top-left corner of the terminal) for drawing.

        // Iterate over each character in the frame buffer and print it.
        // This effectively draws the entire frame buffer to the terminal screen.
        for (int k = 0; k < windowWidth * windowHeight; k++)
        {
            // Print each character. If at the end of a row (not divisible by windowWidth), print the character.
            // Otherwise, insert a newline to move to the next row.
            putchar(k % windowWidth ? frameBuffer[k] : '\n');
        }

        if (isKeyPressed())
        {          // Check if '1' is pressed
            break; // Exit the loop if '1' is pressed
        }
        incrementRotationAngles(); // Increment the rotation angles for the next frame to animate the cube.
        wait(1000);                // Wait for a short period (1000 microseconds) before drawing the next frame.
    }

    resetTerminalMode(); // Reset terminal to original settings before exiting.

    return 0;
}
