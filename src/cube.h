#ifndef CUBE_H
#define CUBE_H

#include "sdl.h"
#include "std.h"
#include "cubies.h"

#define DEFAULT_POWER 10;

struct cube {
    /**
     * Contains the power of the cube. For example: if it is a 3x3x3, the
     * power is 3.
     */
    int power;
    /**
     * Contains the number of cubies. For example: A 3x3x3 has 26 cubies since
     * we do not count cubies that are inside the cube, that would be
     * inefficient.
     */
    int n_cubies;
    /**
     * Contains a pointer to the table of cubies. This table is dynamically
     * allocated to allow the cube to have any power.
     */
    struct cubies* st_cubies;
    /**
     * Contains the mesh structure used by the cubies.
     */
    struct mesh st_mesh;
    /**
    * Points to the parent SDL context of the cube.
    *
    * TODO: Might use that one later.
    */
    struct sdl_context* st_sdl_parent;
};

/**
 * This function initialises the cube with safe defaults.
 *
 * @param st_cube A pointer to a cube structure.
 */
void cube_init(struct cube* st_cube);

/**
 * This function links the cube with its SDL context.
 *
 * @param st_cube A pointer to a cube structure.
 * @param st_sdl A pointer to a sdl_context structure.
 */
void cube_link_sdl(struct cube* st_cube, struct sdl_context* st_sdl);

/**
 * This function allocates memories to the cube based on its power. It creates
 * the table containing the cubies.
 *
 * @param st_cube A pointer to a cube structure.
 */
void cube_allocate(struct cube* st_cube);

/**
 * This function setups the cube. It initialises the position and the rotation
 * of the cubies.
 *
 * @param st_cube A pointer to a cube structure.
 */
void cube_setup(struct cube* st_cube);

/**
 * This function destroys the cube. It frees all allocated memories.
 *
 * @param st_cube A pointer to a cube structure.
 */
void cube_destroy(struct cube* st_cube);

/**
 * This function draws the cube at the origin of the current matrix.
 *
 * @param st_cube A pointer to a cube structure.
 */
void cube_draw(struct cube* st_cube);

#endif
