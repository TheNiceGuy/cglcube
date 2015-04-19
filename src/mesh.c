#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <GL/gl.h>
#include "config.h"
#include "std.h"
#include "mesh.h"

void mesh_init(struct mesh* st_mesh, char* file) {
    sprintf(st_mesh->file, "model/%s", file);
    st_mesh->nvert = 0;
    st_mesh->nnorm = 0;
    st_mesh->nface = 0;
    st_mesh->cface = 0;

    st_mesh->vertices = NULL;
    st_mesh->indices  = NULL;
    st_mesh->colorpointer  = NULL;
    st_mesh->vertexpointer = NULL;

    mesh_load(st_mesh);
}

void mesh_load(struct mesh* st_mesh) {
    st_mesh->object = fopen(st_mesh->file, "r");
    if(st_mesh->object == NULL)
        printf("Impossible to open the file!\n");

    mesh_alloc(st_mesh);
    mesh_parse_file(st_mesh);

    fclose(st_mesh->object);
}

void mesh_parse_file(struct mesh* st_mesh) {
    char buffer[READ_BUFFER];
    char** argv;
    int argc;
    int osize, nsize;
    int vertc;
    int i;

    GLfloat color[3];

    rewind(st_mesh->object);
    osize = 0;
    nsize = 0;
    vertc = 0;
    st_mesh->cface = 0;
    do {
        osize = nsize;
        nsize = read_buffer(st_mesh->object, buffer, nsize);

        parse_argument(&argv, &argc, buffer);

        for(i = 0; i < argc; i++) {
            if(strcmp(argv[i], "usemtl") == 0) {
                mtl_set(&st_mesh->st_mtl, argv[i+1], color);
            } else if(strcmp(argv[i], "v") == 0) {
                st_mesh->vertices[vertc+0] = strtof(argv[i+1], NULL);
                st_mesh->vertices[vertc+1] = strtof(argv[i+2], NULL);
                st_mesh->vertices[vertc+2] = strtof(argv[i+3], NULL);
                vertc=vertc+3; i=i+3;
            } else if(strcmp(argv[i], "f") == 0) {
                mesh_parse_face(st_mesh, &argv[i+1], color);

                i=i+3;
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

void mesh_parse_face(struct mesh* st_mesh, char** coor, GLfloat color[3]) {
    char** argv;
    int argc;
    int i, j, k[3];

    for(i = 0; i < 3; i++) {
        for(j = 0; j < strlen(coor[i]); j++) {
            /*
             * TODO: Add support texture coordinates.
             */
            if(coor[i][j] == '/') {
                coor[i][j] = ' ';
            }
        }
        parse_argument(&argv, &argc, coor[i]);

        for(j = 0; j < argc; j++) {
            /*
             * First one is the vertex index.
             */
            if(j == 0)
                k[i] = atoi(argv[j])-1;
        }

        /*
         * Free dynamically allocated argument table.
         */
        for(j = 0; j < argc; j++)
            free(argv[j]);
        free(argv);
    }

    st_mesh->indices[st_mesh->cface+0] = st_mesh->cface+0;
    st_mesh->vertexpointer[3*st_mesh->cface+0] = st_mesh->vertices[3*k[0]+0];
    st_mesh->vertexpointer[3*st_mesh->cface+1] = st_mesh->vertices[3*k[0]+1];
    st_mesh->vertexpointer[3*st_mesh->cface+2] = st_mesh->vertices[3*k[0]+2];
    st_mesh->colorpointer[3*st_mesh->cface+0] = color[0];
    st_mesh->colorpointer[3*st_mesh->cface+1] = color[1];
    st_mesh->colorpointer[3*st_mesh->cface+2] = color[2];

    st_mesh->indices[st_mesh->cface+1] = st_mesh->cface+1;
    st_mesh->vertexpointer[3*st_mesh->cface+3] = st_mesh->vertices[3*k[1]+0];
    st_mesh->vertexpointer[3*st_mesh->cface+4] = st_mesh->vertices[3*k[1]+1];
    st_mesh->vertexpointer[3*st_mesh->cface+5] = st_mesh->vertices[3*k[1]+2];
    st_mesh->colorpointer[3*st_mesh->cface+3] = color[0];
    st_mesh->colorpointer[3*st_mesh->cface+4] = color[1];
    st_mesh->colorpointer[3*st_mesh->cface+5] = color[2];

    st_mesh->indices[st_mesh->cface+2] = st_mesh->cface+2;
    st_mesh->vertexpointer[3*st_mesh->cface+6] = st_mesh->vertices[3*k[2]+0];
    st_mesh->vertexpointer[3*st_mesh->cface+7] = st_mesh->vertices[3*k[2]+1];
    st_mesh->vertexpointer[3*st_mesh->cface+8] = st_mesh->vertices[3*k[2]+2];
    st_mesh->colorpointer[3*st_mesh->cface+6] = color[0];
    st_mesh->colorpointer[3*st_mesh->cface+7] = color[1];
    st_mesh->colorpointer[3*st_mesh->cface+8] = color[2];
    st_mesh->cface=st_mesh->cface+3;
}

void mesh_alloc(struct mesh* st_mesh) {
    char buffer[READ_BUFFER];
    char** argv;
    int argc;
    int osize, nsize, i;

    rewind(st_mesh->object);
    osize = 0;
    nsize = 0;
    do {
        osize = nsize;
        nsize = read_buffer(st_mesh->object, buffer, nsize);

        parse_argument(&argv, &argc, buffer);

        for(i = 0; i < argc; i++)
            if(strcmp(argv[i], "mtllib") == 0) {
                mtl_init(&st_mesh->st_mtl, argv[i+1]);
            } else if(strcmp(argv[i], "v") == 0) {
                st_mesh->nvert++;
            } else if(strcmp(argv[i], "vn") == 0) {
                st_mesh->nnorm++;
            } else if(strcmp(argv[i], "f") == 0) {
                st_mesh->nface++;
            }

        /*
         * Free dynamically allocated argument table.
         */
        for(i = 0; i < argc; i++)
            free(argv[i]);
        free(argv);
    } while(nsize != osize);

    st_mesh->vertices = malloc(3*st_mesh->nvert*sizeof(GLfloat));
    st_mesh->indices  = malloc(3*st_mesh->nface*sizeof(GLfloat));
    st_mesh->colorpointer  = malloc(9*st_mesh->nface*sizeof(GLfloat));
    st_mesh->vertexpointer = malloc(9*st_mesh->nface*sizeof(GLfloat));
}

void mesh_free(struct mesh* st_mesh) {
    free(st_mesh->vertices);
    free(st_mesh->indices);
    free(st_mesh->colorpointer);
    free(st_mesh->vertexpointer);

    mtl_free(&st_mesh->st_mtl);
}
