#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "config.h"
#include "render.h"

/**
 * This data structure contains variables about the main SDL context of the
 * game. Most of the game depends on it because it controls the threading, the
 * drawing and the keyboard events.
 */
struct sdl_context {
    /**
     * Contains a bool defined in config.h about the state of the
     * SDL context, whether or not it is running.
     */
    int running;
    /**
     * Contains the rendering context.
     */
    struct render_context st_render;

    SDL_Event event;
    SDL_version version_compiled;
    SDL_version version_linked;
    SDL_Window *window;
    SDL_GLContext window_glcontext;
};

/**
 * This function print to stdout the version of the linked and compiled versions
 * of SDL.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 */
void sdl_display_version(struct sdl_context* st_sdl);

/**
 * This function print to stdout the version OpenGL in SDL. It must be called in
 * the same thread as SDL and OpenGL initialisations.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 */
void sdl_opengl_version(struct sdl_context* st_sdl);

/**
 * This function initialises the SDL context with safe defaults.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 */
void sdl_init(struct sdl_context* st_sdl);

/**
 * This function starts the whole SDL engine. It creates the rendering thread
 * and the OpenGL context.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @return An error code defined in config.h.
 */
int sdl_start(struct sdl_context* st_sdl);

/**
 * This function stops the SDL engine and the rendering thread.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @return An error code defined in config.h.
 */
int sdl_stop(struct sdl_context* st_sdl);

/**
 * This function creates the OpenGL context.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @return An error code defined in config.h.
 */
int sdl_create_opengl(struct sdl_context* st_sdl);

/**
 * This function frees SDL's memories by detroying its contexts. It quits SDL as
 * well.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @return An error code defined in config.h.
 */
int sdl_free(struct sdl_context* st_sdl);

/**
 * This function handles SDL's events.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @return An error code defined in config.h.
 */
int sdl_handle_event(struct sdl_context* st_sdl);

/** @file */
#endif
