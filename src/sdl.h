#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/glew.h>
#include "render.h"
#include "camera.h"

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
     * Contains a bool defined in config.h that tells if the game is
     * in fullscreen mode.
     */
    int fullscreen;
    /**
     * Contains the rendering context.
     */
    struct render_context st_render;

    SDL_Event event;
    SDL_version version_compiled;
    SDL_version version_linked;
    SDL_Window *window;
    SDL_GLContext window_glcontext;
    SDL_GLContext render_glcontext;
    SDL_atomic_t glcontext_ready;
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
 * This function toggles the fullscreen mode in the game.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @return An error code defined in config.h.
 */
int sdl_fullscreen(struct sdl_context* st_sdl);

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

/**
 * This function handles window events like resizing.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @param window The structure about the event.
 * @return An error code defined in config.h.
 */
int sdl_handle_window(struct sdl_context* st_sdl, SDL_WindowEvent window);

/**
 * This function handles keyboard events like a keypress.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @param window The structure about the event.
 * @return An error code defined in config.h.
 */
int sdl_handle_key(struct sdl_context* st_sdl, SDL_Keycode key);

/**
 * This function handles mouse button events like a click.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @param window The structure about the event.
 * @return An error code defined in config.h.
 */
int sdl_handle_button(struct sdl_context* st_sdl, SDL_MouseButtonEvent mouse);

/**
 * This function handles mouse motion events like moving the mouse.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @param window The structure about the event.
 * @return An error code defined in config.h.
 */
int sdl_handle_motion(struct sdl_context* st_sdl, SDL_MouseMotionEvent motion);

/**
 * This function handles mouse wheel events like scrolling.
 *
 * @param st_sdl A pointer to an sdl_context structure.
 * @param window The structure about the event.
 * @return An error code defined in config.h.
 */
int sdl_handle_wheel(struct sdl_context* st_sdl, SDL_MouseWheelEvent wheel);

/** @file */
#endif
