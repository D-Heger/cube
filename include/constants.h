/**
 * @file constants.h
 * @author D. Heger
 * @brief Project-wide constants and configuration values
 * @version 1.1.0
 * @date 2025-08-05
 */

#ifndef CONSTANTS_H
#define CONSTANTS_H

/* Display and rendering constants */
#define WINDOW_WIDTH        80                                  /**< Terminal window width in characters */
#define WINDOW_HEIGHT       22                                  /**< Terminal window height in characters */
#define BUFFER_SIZE         (WINDOW_WIDTH * WINDOW_HEIGHT)      /**< Total buffer size */

/* Cube geometry constants */
#define DEFAULT_CUBE_WIDTH      10.0f                           /**< Default cube width/height/depth */
#define CUBE_INCREMENT_STEP     1.0f                            /**< Step size for cube surface iteration */

/* Rendering constants */
#define DEFAULT_VIEWER_DISTANCE     60                          /**< Distance from viewer to cube center */
#define DEFAULT_PROJECTION_SCALE    40.0f                       /**< Projection scale factor for perspective */
#define BACKGROUND_CHARACTER        ' '                         /**< Character used for background/empty space */

/* Animation constants */
#define ROTATION_INCREMENT      0.005f                          /**< Rotation speed per frame (radians) */
#define FRAME_DELAY_MICROSEC    1000                            /**< Delay between frames in microseconds */

/* Input constants */
#define EXIT_KEY_CODE           '1'                             /**< Key code to exit the program */

/* Terminal control sequences */
#define CLEAR_SCREEN_SEQ        "\x1b[2J"                       /**< ANSI sequence to clear screen */
#define HOME_CURSOR_SEQ         "\x1b[H"                        /**< ANSI sequence to move cursor home */

#endif /* CONSTANTS_H */