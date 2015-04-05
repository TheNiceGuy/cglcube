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

#endif
