#include <stdlib.h>
#include "config.h"
#include "std.h"

void* mono_to_RGBA(void* source, int w, int h) {
    unsigned char* pixels;
    int area, i;

    area = w*h;
    pixels = malloc(area*4);

    for(i = 0; i < area; i++) {
        if(*((char*)source+i) == 1) {
            pixels[4*i+0] = 0;
            pixels[4*i+1] = 0;
            pixels[4*i+2] = 0;
            pixels[4*i+3] = 255;
        } else if(*((char*)source+i) == 0) {
            pixels[4*i+0] = 255;
            pixels[4*i+1] = 255;
            pixels[4*i+2] = 255;
            pixels[4*i+3] = 0;
        }
    }

    return (void*)pixels;
}
