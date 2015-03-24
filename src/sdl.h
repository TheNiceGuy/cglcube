#ifndef SDL_H
#define SDL_H

#include <SDL2/SDL.h>
#include <GL/gl.h>
#include "config.h"
#include "render.h"

struct sdl_context {
    int running;

    struct render_context st_render;

    SDL_Event event;
    SDL_version version_compiled;
    SDL_version version_linked;
    SDL_Window *window;
    SDL_GLContext window_glcontext;
};

void sdl_display_version(struct sdl_context* st_sdl);
void sdl_opengl_version(struct sdl_context* st_sdl);
void sdl_init(struct sdl_context* st_sdl);
int sdl_start(struct sdl_context* st_sdl);
int sdl_stop(struct sdl_context* st_sdl);
int sdl_create_opengl(struct sdl_context* st_sdl);
int sdl_free(struct sdl_context* st_sdl);
int sdl_handle_event(struct sdl_context* st_sdl);

#endif
