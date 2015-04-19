#include <stdlib.h>
#include <string.h>
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

void parse_argument(char*** argv, int* argc, char* string) {
    int i, j, k;

    /*
     * Strip useless spaces at the beginning.
     */
    i = 0;
    while(string[i] != '\0') {
        i++;
        if(string[i-1] == ' ') {
            string++;
            i = 0;
        } else
            break;
    }

    /*
     * Strip useless spaces at the end.
     */
    i = 0;
    while(string[i] != '\0')
        i++;
    i--;
    k = i;
    while(string[i] != '\0') {
        i--;
        if(string[i+1] == ' ') {
            string[i+1] = '\0';
            k--;
            i = k;
        } else
            break;
    }

    /*
     * Remove useless spaces between arguments.
     */
    i = 0;
    while(string[i] != '\0') {
        if(string[i] == ' ' && string[i+1] == ' ') {
            k = i;
            while(string[k] != '\0') {
                string[k] = string[k+1];
                k++;
            }
            i = 0;
        }

        i++;
    }

    /*
     * Find the number of argument by counting the number of separations.
     */
    i = 0;
    j = 0;
    while(string[i] != '\0') {
        if(string[i] == ' ')
            j++;
        i++;
    }
    j++;

    *argv = malloc(j*sizeof(char*));

    /*
     * Dynamically allocate memory for each argument.
     */
    i = 0;
    j = 0;
    k = 1;
    while(string[i] != '\0') {
        if(string[i+1] != ' ' && string[i+1] != '\0')
            k++;
        else {
            (*argv)[j] = malloc(k*sizeof(char)+1);
            (*argv)[j][k] = '\0';

            memcpy((*argv)[j], string+i-k+1, k);

            j++;
            k = 0;
        }
        i++;
    }

    *argc = j;
}

int read_buffer(FILE* object, char* buffer, int pos) {
    int size, nsize, i;

    /*
     * Get last line that can fits in the buffer.
     */
    size = fread(buffer, 1, READ_BUFFER, object);
    for(i = 0; i < size; i++)
        if(buffer[i] == '\n')
            nsize = i;

    /*
     * Read until the last possible newline to prevent cutoff.
     */
    fseek(object, pos, SEEK_SET);
    size = fread(buffer, 1, nsize, object);
    buffer[size] = '\0';

    /*
     * Replace new lines by spaces.
     */
    pos = pos+size;
    for(i = 0; i < size; i++)
        if(buffer[i] == '\n')
            buffer[i] = ' ';

    return pos;
}
