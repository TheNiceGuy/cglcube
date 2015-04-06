#ifndef STD_H
#define STD_H

struct dimension {
    int w;
    int h;
};

struct point {
    double x;
    double y;
    double z;
};

/**
 * This function converts an image mono-colorized into an RGBA-colorized image.
 * The returned image must be freed after it is no longer used.
 *
 * @param st_sdl A pointer to the pixels that need to be converted.
 * @param w The width of the image.
 * @param h The height of the image.
 * @return A void pointer to the RGBA image.
 */
void* mono_to_RGBA(void* source, int w, int h);

/**
 * This function parses the arguments from a string into multiple dynamically
 * allocated string. All arguments are returned in a argv and argc contains the
 * number of arguments parsed.
 *
 * @param argv A pointer to an array of string.
 * @param argc A pointer to an int.
 * @param string A pointer to the string that needs parsing.
 */
void parse_argument(char*** argv, int* argc, char* string);

#endif
