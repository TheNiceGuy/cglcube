#ifndef MESH_H
#define MESH_H

#include <stdio.h>
#include "material.h"

struct mesh {
    /**
     * Contains the name of the file that contains the materials library.
     */
    char file[64];
    /**
     * Contains the number of possible vertices.
     */
    int nvert;
    /**
     * Contains the number of possible normals.
     */
    int nnorm;
    /**
     * Contains the number of faces.
     */
    int nface;
    /**
     * Contains the index of the next face that will be written.
     */
    int cface;
    /*
     * Contains the size of all the vertex/normal/color.
     */
    int elementsize;
    /**
     * Contains the VBO ID used to render the mesh.
     */
    GLuint vboID;
    /**
     * Contains a pointer to the possible vertices.
     */
    GLfloat* vertices;
    /**
     * Contains a pointer to the possible vertices.
     */
    GLfloat* normals;
    /**
     * Contains a pointer to the rendered vertices.
     */
    GLfloat* vertexpointer;
    /**
     * Contains a pointer to the rendered normals.
     */
    GLfloat* normalpointer;
    /**
     * Contains a pointer to the rendered colors.
     */
    GLfloat* colorpointer;
    /**
     * Contains the file descriptor of the mesh.
     */
    FILE* object;
    /**
     * Contains the materials library.
     */
    struct mtl st_mtl;
};

/**
 * This function initialises the mesh with safe defaults. It loads the
 * mesh using mesh_load().
 *
 * @param st_mesh A pointer to a mesh structure.
 * @param file A pointer to the string that contains the file's name.
 */
void mesh_init(struct mesh* st_mesh, char* file);

/**
 * This function loads mesh. It allocates the memories using mesh_alloc()
 * and parse the file using mesh_parse_file().
 *
 * @param st_mesh A pointer to a mesh structure.
 */
void mesh_load(struct mesh* st_mesh);

/**
 * This function parses the data inside the file into an mesh structure.
 *
 * @param st_mesh A pointer to a mesh structure.
 */
void mesh_parse_file(struct mesh* st_mesh);

/**
 * This function parses a face inside the file.
 *
 * @param st_mesh A pointer to a mesh structure.
 * @param coor A pointer to an array of string containing the data of a face.
 * @param color A pointer to the current color defined in the file.
 */
void mesh_parse_face(struct mesh* st_mesh, char** coor, GLfloat color[3]);

/**
 * This function allocates memories based from the data in the file for a mesh
 * structure.
 *
 * @param st_mesh A pointer to a mesh structure.
 */
void mesh_alloc(struct mesh* st_mesh);

/**
 * This function frees all allocated memories of a mesh.
 *
 * @param st_mesh A pointer to a mesh structure.
 */
void mesh_free(struct mesh* st_mesh);

#endif
