#ifndef MATERIAL_H
#define MATERIAL_H

#include <stdio.h>
#include <GL/gl.h>

struct mtl {
    /**
     * Contains the name of the file that contains the materials library.
     */
    char file[64];
    /**
     * Contains a table containing the name of all the material found in the
     * library.
     */
    char** name;
    /**
     * Contains the number of material found in the library.
     */
    int nmtl;
    /**
     * Contains a pointer to the diffuse colors of all material found in the
     * library. Each material has a Red, Green and Blue value.
     */
    GLfloat* diffuse;
    /**
     * Contains the file descriptor of the materials library.
     */
    FILE* object;
};

/**
 * This function initialises the library with safe defaults. It loads the
 * library using mtl_load().
 *
 * @param st_mtl A pointer to a mtl structure.
 * @param st_mtl A pointer to the string that contains the file's name.
 */
void mtl_init(struct mtl* st_mtl, char* file);

/**
 * This function loads the library. It allocates the memories using mtl_alloc()
 * and parse the file using mtl_parse_file().
 *
 * @param st_mtl A pointer to a mtl structure.
 */
void mtl_load(struct mtl* st_mtl);

/**
 * This function parses the data inside the file into an mtl structure.
 *
 * @param st_mtl A pointer to a mtl structure.
 */
void mtl_parse_file(struct mtl* st_mtl);

/**
 * This function allocates memories based from the data in the file for an
 * mtl structure.
 *
 * @param st_mtl A pointer to a mtl structure.
 */
void mtl_alloc(struct mtl* st_mtl);

/**
 * This function frees all allocated memories by an mtl structure.
 *
 * @param st_mtl A pointer to a mtl structure.
 */
void mtl_free(struct mtl* st_mtl);

/**
 * This function sets the color to one material inside the library. It only sets
 * the diffuse color right now.
 *
 * TODO: Add support for specular, ambiant, etc.
 *
 * @param st_mtl A pointer to a mtl structure.
 * @param st_mtl A pointer to a string containing the name of the requested
 *               material.
 * @param st_mtl A pointer to an RGB array.
 */
void mtl_use(struct mtl* st_mtl, char* name, GLfloat* color);

/**
 * This function changes the color of a material inside the library. It only
 * changes the diffuse color right now.
 *
 * TODO: Add support for specular, ambiant, etc.
 *
 * @param st_mtl A pointer to a mtl structure.
 * @param st_mtl A pointer to a string containing the name of the requested
 *               material.
 * @param st_mtl A pointer to an RGB array.
 */
void mtl_set(struct mtl* st_mtl, char* name, GLfloat* color);

#endif
