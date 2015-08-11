#include <GL/gl.h>
#include <math.h>
#include "config.h"
#include "sdl.h"
#include "draw.h"
#include "cube.h"

void cube_init(struct cube* st_cube, int power) {
    st_cube->power = power;
    st_cube->mutex = SDL_CreateMutex();

    SDL_LockMutex(st_cube->mutex);
    cube_create(st_cube);
    SDL_UnlockMutex(st_cube->mutex);
}

void cube_link_sdl(struct cube* st_cube, struct sdl_context* st_sdl) {
    st_cube->st_sdl_parent = st_sdl;
}

void cube_create(struct cube* st_cube) {
    mesh_init(&st_cube->st_mesh, "cube_test.obj");
    cube_allocate(st_cube);
    cube_setup(st_cube);
}

void cube_destroy(struct cube* st_cube) {
    cube_free(st_cube);
    SDL_DestroyMutex(st_cube->mutex);
}

void cube_allocate(struct cube* st_cube) {
    int total, inside;

    total  = pow(st_cube->power  , 3);
    inside = pow(st_cube->power-2, 3);
    if(inside < 0)
        inside = 0;

    st_cube->n_cubies = total-inside;
    st_cube->st_cubies = malloc(st_cube->n_cubies*sizeof(struct cubies));
}

void cube_free(struct cube* st_cube) {
    int i;

    mesh_free(&st_cube->st_mesh);

    for(i = 0; i < st_cube->n_cubies; i++)
        cubies_free(&st_cube->st_cubies[i]);
    free(st_cube->st_cubies);
}

void cube_setup(struct cube* st_cube) {
    int i, x, y, z;

    i = 0;
    for(z = 0; z < st_cube->power; z++) {
        for(y = 0; y < st_cube->power; y++) {
            for(x = 0; x < st_cube->power; x++) {
                if(x == 0 || x == st_cube->power-1 ||
                   y == 0 || y == st_cube->power-1 ||
                   z == 0 || z == st_cube->power-1)
                {
                    cubies_init(&st_cube->st_cubies[i]);
                    cubies_set_mesh(&st_cube->st_cubies[i], &st_cube->st_mesh);
                    cubies_set_pos(&st_cube->st_cubies[i], x, y, z);

                    i++;
                }
            }
        }
    }
}

void cube_change_power(struct cube* st_cube, int power) {
    st_cube->power = power;

    SDL_LockMutex(st_cube->mutex);
    cube_free(st_cube);
    cube_create(st_cube);
    SDL_UnlockMutex(st_cube->mutex);
}

void cube_change_color(struct cube* st_cube, char* name, int r, int g, int b) {
    GLfloat color[3];

    color[0] = (GLfloat)r/255;
    color[1] = (GLfloat)g/255;
    color[2] = (GLfloat)b/255;

    mtl_set(&st_cube->st_mesh.st_mtl, name, color);
    mesh_update(&st_cube->st_mesh);
}

void cube_draw(struct cube* st_cube) {
    int i;

    glTranslatef(-(float)st_cube->power/2,
                 -(float)st_cube->power/2,
                 -(float)st_cube->power/2);

    SDL_LockMutex(st_cube->mutex);
    for(i = 0; i < st_cube->n_cubies; i++) {
        cubies_draw(&st_cube->st_cubies[i]);
    }
    SDL_UnlockMutex(st_cube->mutex);
}
