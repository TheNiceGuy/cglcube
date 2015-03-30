#ifndef RENDER_H
#define RENDER_H

#include <SDL2/SDL_ttf.h>
#include <pthread.h>
#include "camera.h"
#include "text.h"

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
     * Contains the X resolution of the window.
     */
    int window_x;
    /**
     * Contains the Y resolution of the window.
     */
    int window_y;
    /**
     * Contains the X resolution before the fullscreen.
     */
    int old_x;
    /**
     * Contains the Y resolution before the fullscreen.
     */
    int old_y;
    /**
     * Contains the current fps.
     */
    float fps;
    /**
     * Contains the ratio X/Y.
     */
    double ratio;
    /**
     * Contains the camera context.
     */
    struct camera_context st_camera;
    /**
     * Contains the text rendered on the screen.
     */
    struct text st_overlay_text;
    /**
     * Points to the parent SDL context of the rendering context.
     */
    struct sdl_context* st_sdl_parent;
    /**
     * Contains the thread of the rendering context.
     */
    pthread_t thread;
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

/**
 * This function resizes the region that is rendered by the engine.
 *
 * @param st_sdl A pointer to a render_context structure.
 */
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
 * @param st_sdl A pointer to a render_context structure.
 * @return An error code defined in config.h.
 */
int render_start(struct render_context* st_render);

/**
 * This function stops and joins the rendering thread.
 *
 * @param st_sdl A pointer to a render_context structure.
 * @return An error code defined in config.h.
 */
int render_stop(struct render_context* st_render);

/**
 * This function renders the information on the screen.
 *
 * @param st_sdl A pointer to a render_context structure.
 * @return An error code defined in config.h.
 */
int render_info(struct render_context* st_render);

/**
 * This function renders the overlay on the screen. It setups opengl and calls
 * subfunctions
 *
 * @param st_sdl A pointer to a render_context structure.
 * @return An error code defined in config.h.
 */
int render_overlay(struct render_context* st_render);

/**
 * This function does all the calls to the multiple drawing functions.
 *
 * @param st_sdl A pointer to a render_context structure.
 * @return An error code defined in config.h.
 */
int render_window(struct render_context* st_render);

/** @file */
#endif
