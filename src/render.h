#ifndef RENDER_H
#define RENDER_H

#include <pthread.h>
#include "camera.h"

/**
 * This data structure contains variables about the rendering context of the
 * game.
 */
struct render_context {
    /**
    * Contains a bool defined in config.h about the state of the
    * rendering context, whether or not it is running.
    */
    int running;
    /**
    * Contains the x resolution of the window.
    */
    int window_x;
    /**
    * Contains the y resolution of the window.
    */
    int window_y;
    /**
    * Contains the ratio x/y.
    */
    double ratio;
    /**
     * Contains the camera context.
     */
    struct camera_context st_camera;
    /**
    * Points to the parent SDL context of the rendering context.
    */
    struct sdl_context* st_sdl_parent;
    /**
    * Contains the thread of the rendering context.
    */
    pthread_t thread;
    /**
    * Contains the mutex for locking the rendering context.
    */
    pthread_mutex_t thread_mutex;
};

/**
 * This function initialises the rendering context.
 *
 * @param st_sdl A pointer to a render_context structure.
 * @param x The x resolution of the window.
 * @param y The x resolution of the window.
 */
void  render_init(struct render_context* st_render, int x, int y);

/**
 * This function links the rendering context with its SDL context.
 *
 * @param st_sdl A pointer to a render_context structure.
 * @param st_sdl A pointer to a sdl_context structure.
 */
void  render_link_sdl(struct render_context* st_render, struct sdl_context* st_sdl);

void  render_resize_window(struct render_context* st_render, int x, int y);

/**
 * This function is the main rendering method. It draws and refreshes the window
 * at a particular rate.
 *
 * @param st_sdl A void pointer to a render_context structure.
 * @return NULL
 */
void* render_timer(void* st_render_ptr);

/**
 * This function starts the rendering thread.
 *
 * @param st_sdl A void pointer to a render_context structure.
 * @return An error code defined in config.h.
 */
int render_start(struct render_context* st_render);

/**
 * This function stops and joins the rendering thread.
 *
 * @param st_sdl A void pointer to a render_context structure.
 * @return An error code defined in config.h.
 */
int render_stop(struct render_context* st_render);

/**
 * This function does all the calls to the multiple drawing functions.
 *
 * @param st_sdl A void pointer to a render_context structure.
 * @return An error code defined in config.h.
 */
int render_window(struct render_context* st_render);

/** @file */
#endif
