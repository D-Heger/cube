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
#define DEFAULT_WINDOW_WIDTH    80      /**< Default terminal window width in characters */
#define DEFAULT_WINDOW_HEIGHT   22      /**< Default terminal window height in characters */
#define MIN_WINDOW_WIDTH        10      /**< Minimum supported window width */
#define MIN_WINDOW_HEIGHT       5       /**< Minimum supported window height */
#define MAX_WINDOW_WIDTH        200     /**< Maximum supported window width */
#define MAX_WINDOW_HEIGHT       100     /**< Maximum supported window height */

/* Memory allocation constants */
#define ALLOCATION_SUCCESS      0       /**< Successful memory allocation return code */
#define ALLOCATION_FAILURE      -1      /**< Failed memory allocation return code */

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