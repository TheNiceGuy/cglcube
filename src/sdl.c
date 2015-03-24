#include "sdl.h"

void sdl_display_version(struct sdl_context* st_sdl) {
    SDL_VERSION(&st_sdl->version_compiled);
    SDL_GetVersion(&st_sdl->version_linked);

    printf("SDL compiled version:\t %d.%d.%d\n",
        st_sdl->version_compiled.major,
        st_sdl->version_compiled.minor,
        st_sdl->version_compiled.patch);
    printf("SDL linked version:\t %d.%d.%d\n",
        st_sdl->version_linked.major,
        st_sdl->version_linked.minor,
        st_sdl->version_linked.patch);
}

void sdl_opengl_version(struct sdl_context* st_sdl) {
    printf("OpenGL version:\t\t %s\n", glGetString(GL_VERSION));
}

void sdl_init(struct sdl_context* st_sdl) {
    st_sdl->running = FALSE;
}

int sdl_start(struct sdl_context* st_sdl) {
    if(st_sdl->running == TRUE)
        return FAILED;

    render_link_sdl(&st_sdl->st_render, st_sdl);
    render_start(&st_sdl->st_render);

    st_sdl->running = TRUE;
    return SUCCESS;
}

int sdl_stop(struct sdl_context* st_sdl) {
    if(st_sdl->running == FALSE)
        return FAILED;
    render_stop(&st_sdl->st_render);

    st_sdl->running = FALSE;
    return SUCCESS;
}

int sdl_create_opengl(struct sdl_context* st_sdl) {
    pthread_mutex_lock(&st_sdl->st_render.thread_mutex);

    if(SDL_Init(SDL_INIT_VIDEO) != SUCCESS) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        exit(FAILED);
    }

    st_sdl->window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                      st_sdl->st_render.window_x, st_sdl->st_render.window_y,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(st_sdl->window == NULL) {
        printf("Can't create SDL window: %s\n", SDL_GetError());
        exit(FAILED);
    }
    st_sdl->window_glcontext = SDL_GL_CreateContext(st_sdl->window);
    pthread_mutex_unlock(&st_sdl->st_render.thread_mutex);

    return 0;
}

int sdl_free(struct sdl_context* st_sdl) {
    SDL_GL_DeleteContext(st_sdl->window_glcontext);
    SDL_DestroyWindow(st_sdl->window);
    SDL_Quit();

    return 0;
}

int sdl_handle_event(struct sdl_context* st_sdl) {
    if(st_sdl->running == FALSE)
        return FAILED;

    while (st_sdl->running == TRUE) {
        SDL_WaitEvent(&st_sdl->event);

        switch(st_sdl->event.type) {
        case SDL_QUIT:
            sdl_stop(st_sdl); break;

        case SDL_KEYDOWN:
            switch(st_sdl->event.key.keysym.sym) {
            case SDLK_ESCAPE:
                sdl_stop(st_sdl); break;
            }
        }
	}

    return 0;
}
