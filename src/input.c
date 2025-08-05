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

void setNonBlockingMode(void)
{
    struct termios ttystate;

    /* Attempt to get current terminal state */
    if (tcgetattr(STDIN_FILENO, &ttystate) != 0) {
        return; /* Failed to get terminal attributes */
    }

    /* Configure non-canonical mode */
    ttystate.c_lflag &= ~(ICANON | ECHO);
    ttystate.c_cc[VMIN] = 1;

    /* Apply new terminal settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

void resetTerminalMode(void)
{
    struct termios ttystate;
    
    /* Attempt to get current terminal state */
    if (tcgetattr(STDIN_FILENO, &ttystate) != 0) {
        return; /* Failed to get terminal attributes */
    }
    
    /* Restore canonical mode and echo */
    ttystate.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &ttystate);
}

int isKeyPressed(void)
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    /* Save current terminal settings */
    if (tcgetattr(STDIN_FILENO, &oldt) != 0) {
        return 0; /* Failed to get terminal attributes */
    }
    newt = oldt;

    /* Configure non-blocking mode */
    newt.c_lflag &= ~(ICANON | ECHO);
    if (tcsetattr(STDIN_FILENO, TCSANOW, &newt) != 0) {
        return 0; /* Failed to set terminal attributes */
    }
    
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    if (oldf == -1) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /* Restore on error */
        return 0;
    }

    /* Enable non-blocking read */
    if (fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK) == -1) {
        tcsetattr(STDIN_FILENO, TCSANOW, &oldt); /* Restore on error */
        return 0;
    }

    ch = getchar();

    /* Always restore original settings */
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    if (ch == EXIT_KEY_CODE) {
        ungetc(ch, stdin); /* Put character back for potential reuse */
        return 1;
    }

    return 0;
}

void wait(int sleep_microseconds)
{
    if (sleep_microseconds > 0) {
        usleep((useconds_t)sleep_microseconds);
    }
}