#ifndef CONFIG_H
#define CONFIG_H

/**
 * Defines the name of the game.
 */

#if defined(__LINUX__)
    #define NAME "CGLCube-LINUX"
#elif defined(__WIN32__)
    #define NAME "CGLCube-WIN32"
#endif

#define DEFAULT_X 3.5
#define DEFAULT_Y 3.5
#define DEFAULT_Z 3.5

#define ZOOM_MIN 1
#define ZOOM_MAX 60

#define DEFAULT_POWER 0

/**
 * Defines a FALSE boolean.
 */
#define FALSE 0

/**
 * Defines a TRUE boolean.
 */
#define TRUE  1

/**
 * Defines the error code for success.
 */
#define SUCCESS 0

/**
 * Defines the error code for failure.
 */
#define FAIL 1

/**
 * Defines the buffer used for reading.
 */
#define READ_BUFFER 128

/** @file */
#endif
