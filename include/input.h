/**
 * @file input.h
 * @author D. Heger
 * @brief Input handling declarations
 * @version 1.1.0
 * @date 2025-08-05
 */

#ifndef INPUT_H
#define INPUT_H

/**
 * @brief Set the terminal to non-blocking mode to detect key presses.
 *
 * This function configures the terminal to operate in non-canonical mode
 * and disables echo, allowing for immediate key detection without waiting
 * for the Enter key to be pressed.
 */
void setNonBlockingMode();

/**
 * @brief Reset the terminal to its original settings.
 *
 * This function restores the terminal to canonical mode and re-enables echo,
 * returning it to normal command-line behavior. Should be called before
 * program termination to ensure proper terminal state.
 */
void resetTerminalMode();

/**
 * @brief Check if a specific key is pressed without blocking.
 *
 * This function performs a non-blocking check for keyboard input.
 * It specifically looks for the '1' key press to allow user-controlled
 * program termination.
 *
 * @return int Returns 1 if the '1' key is pressed, 0 otherwise.
 */
int isKeyPressed();

/**
 * @brief Waits for the specified number of microseconds.
 *
 * This function provides a delay mechanism for controlling frame rate
 * and animation timing in the cube rotation display.
 *
 * @param sleep The number of microseconds to wait.
 */
void wait(int sleep);

#endif // INPUT_H