#include <stdlib.h>
#include <stdio.h>
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
