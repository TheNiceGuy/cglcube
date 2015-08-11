#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "config.h"
#include "std.h"
#include "material.h"

void mtl_init(struct mtl* st_mtl, char* file) {
    sprintf(st_mtl->file, "model/%s", file);
    st_mtl->nmtl = 0;

    st_mtl->name    = NULL;
    st_mtl->diffuse = NULL;
    st_mtl->object  = NULL;

    mtl_load(st_mtl);
}

void mtl_load(struct mtl* st_mtl) {
    st_mtl->object = fopen(st_mtl->file, "r");
    if(st_mtl->object == NULL)
        printf("Impossible to open the file!\n");

    mtl_alloc(st_mtl);
    mtl_parse_file(st_mtl);

    fclose(st_mtl->object);
}

void mtl_parse_file(struct mtl* st_mtl) {
    char buffer[READ_BUFFER];
    char** argv;
    int argc;
    int osize, nsize, i;
    int mtlc, diffc;

    rewind(st_mtl->object);
    osize = 0;
    nsize = 0;
    mtlc  = 0;
    diffc = 0;
    do {
        osize = nsize;
        nsize = read_buffer(st_mtl->object, buffer, nsize);

        parse_argument(&argv, &argc, buffer);

        for(i = 0; i < argc; i++) {
            if(strcmp(argv[i], "newmtl") == 0) {
                st_mtl->name[mtlc] = malloc(strlen(argv[i+1])+1);
                strcpy(st_mtl->name[mtlc], argv[i+1]);
                mtlc = mtlc+1; i = i+2;
            } else if(strcmp(argv[i], "Kd") == 0) {
                st_mtl->diffuse[diffc+0] = strtof(argv[i+1], NULL);
                st_mtl->diffuse[diffc+1] = strtof(argv[i+2], NULL);
                st_mtl->diffuse[diffc+2] = strtof(argv[i+3], NULL);
                diffc = diffc+3; i = i+3;
            }
        }

        /*
         * Free dynamically allocated argument table.
         */
        for(i = 0; i < argc; i++)
            free(argv[i]);
        free(argv);
    } while(nsize != osize);
}

void mtl_alloc(struct mtl* st_mtl) {
    char buffer[READ_BUFFER];
    char** argv;
    int argc;
    int osize, nsize, i;

    rewind(st_mtl->object);
    osize = 0;
    nsize = 0;
    do {
        osize = nsize;
        nsize = read_buffer(st_mtl->object, buffer, nsize);

        parse_argument(&argv, &argc, buffer);

        for(i = 0; i < argc; i++)
            if(strcmp(argv[i], "newmtl") == 0)
                st_mtl->nmtl++;

        /*
         * Free dynamically allocated argument table.
         */
        for(i = 0; i < argc; i++)
            free(argv[i]);
        free(argv);
    } while(nsize != osize);

    st_mtl->name    = malloc(st_mtl->nmtl*sizeof(char*));
    st_mtl->diffuse = malloc(3*st_mtl->nmtl*sizeof(GLfloat));
}

void mtl_free(struct mtl* st_mtl) {
    int i;

    for(i = 0; i < st_mtl->nmtl; i++)
        free(st_mtl->name[i]);
    free(st_mtl->name);
    free(st_mtl->diffuse);
}

void mtl_use(struct mtl* st_mtl, char* name, GLfloat* color) {
    int i;

    for(i = 0; i < st_mtl->nmtl; i++) {
        if(strcmp(st_mtl->name[i], name) == 0)
            break;
    }

    if(i != st_mtl->nmtl) {
        color[0] = st_mtl->diffuse[3*i+0];
        color[1] = st_mtl->diffuse[3*i+1];
        color[2] = st_mtl->diffuse[3*i+2];
    } else {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
    }
}

void mtl_set(struct mtl* st_mtl, char* name, GLfloat* color) {
    int i;

    for(i = 0; i < st_mtl->nmtl; i++) {
        if(strcmp(st_mtl->name[i], name) == 0)
            break;
    }

    if(i != st_mtl->nmtl) {
        st_mtl->diffuse[3*i+0] = color[0];
        st_mtl->diffuse[3*i+1] = color[1];
        st_mtl->diffuse[3*i+2] = color[2];
    } else {
        color[0] = 0;
        color[1] = 0;
        color[2] = 0;
    }
}
