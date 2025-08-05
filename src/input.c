/**
 * @file input.c
 * @author D. Heger
 * @brief Input handling implementation
 * @version 1.1.0
 * @date 2025-08-05
 */

#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include "input.h"

/**
 * @brief Set the terminal to non-blocking mode to detect key presses.
 *
 * This function configures the terminal to operate in non-canonical mode
 * and disables echo, allowing for immediate key detection without waiting
 * for the Enter key to be pressed. This is essential for real-time input
 * handling in the cube animation.
 */
void setNonBlockingMode()
{
    struct termios ttystate;

    /* get the terminal state */
    tcgetattr(STDIN_FILENO, &ttystate);

    /* turn off canonical mode and echo */
    ttystate.c_lflag &= ~(ICANON | ECHO);
    /* minimum of number input read. */
    ttystate.c_cc[VMIN] = 1;

    /* set the terminal attributes. */
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

/**
 * @brief Reset the terminal to its original settings.
 *
 * This function restores the terminal to canonical mode and re-enables echo,
 * returning it to normal command-line behavior. Should be called before
 * program termination to ensure proper terminal state and prevent issues
 * with subsequent command-line usage.
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
 * This function performs a non-blocking check for keyboard input.
 * It specifically looks for the '1' key press to allow user-controlled
 * program termination. The function temporarily modifies terminal settings
 * to achieve non-blocking behavior, then restores them.
 *
 * @return int Returns 1 if the '1' key is pressed, 0 otherwise.
 */
int isKeyPressed()
{
    /* Non-blocking check for keyboard input */
    struct termios oldt, newt;
    int ch;
    int oldf;

    /* Save old settings */
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    /* Disable buffering */
    newt.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);

    /* Apply non-blocking read */
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    ch = getchar();

    /* Restore old settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == '1')
    {
        ungetc(ch, stdin); /* Put the character back, in case further handling is needed */
        return 1;
    }

    return 0;
}

/**
 * @brief Waits for the specified number of microseconds.
 *
 * This function provides a delay mechanism for controlling frame rate
 * and animation timing in the cube rotation display. It uses the system's
 * usleep function to introduce precise timing delays.
 *
 * @param sleep The number of microseconds to wait.
 */
void wait(int sleep)
{
    usleep(sleep);
}