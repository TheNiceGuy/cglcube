#ifndef CUBIES_H
#define CUBIES_H

#include "std.h"
#include "mesh.h"

struct cubies {
    /**
     * Contains the position of the cubies.
     */
    struct point st_pos;
    /**
     * Contains the mesh used to render to cubies.
     */
    struct mesh* st_mesh;
};

/**
 * This function initialises the cubies with safe defaults.
 *
 * @param st_cubies A pointer to a cubies structure.
 */
void cubies_init(struct cubies* st_cubies);

/**
 * This function sets and loads the mesh into the memory.
 *
 * @param st_cubies A pointer to a cubies structure.
 * @param st_cubies A pointer to a mesh structure.
 */
void cubies_set_mesh(struct cubies* st_cubies, struct mesh* st_mesh);

/**
 * This function frees the allocated memories. Actually, it destroys the mesh
 * since it was dynamically allocated.
 *
 * @param st_cubies A pointer to a cubies structure.
 */
void cubies_free(struct cubies* st_cubies);

/**
 * This function sets the position of the cubies.
 *
 * TODO: Maybe we should use floats here.
 *
 * @param x The x coordinate of the new position.
 * @param y The y coordinate of the new position.
 * @param z The z coordinate of the new position.
 * @param st_cubies A pointer to a cubies structure.
 */
void cubies_set_pos(struct cubies* st_cubies, int x, int y, int z);

/**
 * This function draws the cubies.
 *
 * TODO: We should use vertex buffer in order to optimise the drawing.
 *
 * @param st_cubies A pointer to a cubies structure.
 */
void cubies_draw(struct cubies* st_cubies);

#endif
