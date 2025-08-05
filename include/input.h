/**
 * @file input.h
 * @author D. Heger
 * @brief Terminal input handling and timing functions
 * @version 1.1.0
 * @date 2025-08-05
 */

#ifndef INPUT_H
#define INPUT_H

#include "constants.h"

/**
 * @brief Configure terminal for non-blocking input detection
 *
 * Switches the terminal to non-canonical mode with echo disabled,
 * enabling immediate key detection without requiring Enter key press.
 * This is essential for real-time interactive applications.
 *
 * @note Must call resetTerminalMode() before program exit to restore terminal
 * @note May fail silently if terminal attributes cannot be modified
 */
void setNonBlockingMode(void);

/**
 * @brief Restore terminal to original canonical mode
 *
 * Reverts terminal settings to canonical mode with echo enabled,
 * restoring normal command-line behavior. Critical to call before
 * program termination to prevent terminal corruption.
 *
 * @note Should be called in cleanup routines and signal handlers
 * @note May fail silently if terminal attributes cannot be restored
 */
void resetTerminalMode(void);

/**
 * @brief Non-blocking check for exit key press
 *
 * Performs a non-blocking poll of stdin to detect if the designated
 * exit key has been pressed. Uses temporary terminal mode changes
 * to achieve non-blocking behavior without affecting global state.
 *
 * @return int Returns 1 if EXIT_KEY_CODE is pressed, 0 otherwise
 * 
 * @note Currently hardcoded to detect '1' key (EXIT_KEY_CODE)
 * @note Temporarily modifies terminal settings during execution
 * @note Returns immediately without blocking program execution
 */
int isKeyPressed(void);

/**
 * @brief Precise microsecond delay for animation timing
 *
 * Provides sub-millisecond precision timing delays using the system's
 * usleep() function. Used to control animation frame rate and ensure
 * smooth cube rotation visualization.
 *
 * @param sleep_microseconds Duration to wait in microseconds (1/1,000,000 second)
 *                          Valid range: 0 to 999,999 microseconds
 * 
 * @note Large values may be rounded to system timer resolution
 * @note Function may return early if interrupted by signals
 * @note Parameter renamed for clarity (was 'sleep')
 */
void wait(int sleep_microseconds);

#endif // INPUT_H