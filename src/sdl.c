#include "config.h"
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
    st_sdl->running    = FALSE;
    st_sdl->fullscreen = FALSE;

    render_init(&st_sdl->st_render, DEFAULT_RES_X, DEFAULT_RES_Y);
    render_link_sdl(&st_sdl->st_render, st_sdl);
}

int sdl_start(struct sdl_context* st_sdl) {
    if(st_sdl->running == TRUE)
        return FAILED;

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

    if(TTF_Init() != SUCCESS) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(FAILED);
    }

    st_sdl->window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      st_sdl->st_render.window_x,
                                      st_sdl->st_render.window_y,
                                      SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    if(st_sdl->window == NULL) {
        printf("Can't create SDL window: %s\n", SDL_GetError());
        exit(FAILED);
    }

    st_sdl->window_glcontext = SDL_GL_CreateContext(st_sdl->window);
    glewInit();
    glEnable(GL_DEPTH_TEST);
    SDL_GL_SetSwapInterval(0);

    pthread_mutex_unlock(&st_sdl->st_render.thread_mutex);

    return SUCCESS;
}

int sdl_fullscreen(struct sdl_context* st_sdl) {
    SDL_Rect screen;

    SDL_GetDisplayBounds(0, &screen);

    if(!st_sdl->fullscreen) {
        /*
         * Save the old resolution if the user disable fullscreen
         * later in the execution.
         */
        st_sdl->st_render.old_x = st_sdl->st_render.window_x;
        st_sdl->st_render.old_y = st_sdl->st_render.window_y;

        render_resize_window(&st_sdl->st_render, screen.w, screen.h);
        SDL_SetWindowSize(st_sdl->window, screen.w, screen.h);
        SDL_SetWindowFullscreen(st_sdl->window, SDL_TRUE);
        st_sdl->fullscreen = TRUE;
    } else {
        SDL_SetWindowFullscreen(st_sdl->window, SDL_FALSE);
        SDL_SetWindowSize(st_sdl->window,
                          st_sdl->st_render.old_x,
                          st_sdl->st_render.old_y);
        render_resize_window(&st_sdl->st_render,
                              st_sdl->st_render.old_x,
                              st_sdl->st_render.old_y);
        st_sdl->fullscreen = FALSE;
    }

    return SUCCESS;
}

int sdl_free(struct sdl_context* st_sdl) {
    SDL_GL_DeleteContext(st_sdl->window_glcontext);
    SDL_DestroyWindow(st_sdl->window);
    SDL_Quit();

    return SUCCESS;
}

int sdl_handle_event(struct sdl_context* st_sdl) {
    if(!st_sdl->running)
        return FAILED;

    while (st_sdl->running) {
        SDL_WaitEvent(&st_sdl->event);

        switch(st_sdl->event.type) {
        case SDL_QUIT:
            sdl_stop(st_sdl);
            break;

        case SDL_WINDOWEVENT:
            sdl_handle_window(st_sdl, st_sdl->event.window);
            break;

        case SDL_KEYDOWN:
            sdl_handle_key(st_sdl, st_sdl->event.key.keysym.sym);
            break;

        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
            sdl_handle_button(st_sdl, st_sdl->event.button);
            break;

        case SDL_MOUSEMOTION:
            sdl_handle_motion(st_sdl, st_sdl->event.motion);
            break;

        case SDL_MOUSEWHEEL:
            sdl_handle_wheel(st_sdl, st_sdl->event.wheel);
            break;
        }
    }

    return SUCCESS;
}

int sdl_handle_window(struct sdl_context* st_sdl, SDL_WindowEvent window) {
    switch(window.event) {
    case SDL_WINDOWEVENT_RESIZED:
        render_resize_window(&st_sdl->st_render, window.data1, window.data2);
    }

    return SUCCESS;
}

int sdl_handle_key(struct sdl_context* st_sdl, SDL_Keycode key) {
    switch(key) {
    case SDLK_ESCAPE:
    case SDLK_q:
        sdl_stop(st_sdl);
        break;
    case SDLK_f:
        sdl_fullscreen(st_sdl);
    }

    return SUCCESS;
}

int sdl_handle_button(struct sdl_context* st_sdl, SDL_MouseButtonEvent mouse) {
    switch(mouse.button) {
    case SDL_BUTTON_LEFT:
        switch(mouse.state) {
        case SDL_PRESSED:
            if(!st_sdl->st_render.st_camera.move)
                camera_toggle_move(&st_sdl->st_render.st_camera);
            break;
        case SDL_RELEASED:
            if(st_sdl->st_render.st_camera.move)
                camera_toggle_move(&st_sdl->st_render.st_camera);
            break;
        }
        break;
    case SDL_BUTTON_RIGHT:
        printf("Right button placeholder\n");
        break;
    }

    return SUCCESS;
}

int sdl_handle_motion(struct sdl_context* st_sdl, SDL_MouseMotionEvent motion) {
    if(st_sdl->st_render.st_camera.move) {
        camera_rotate_scene(&st_sdl->st_render.st_camera, motion.xrel);
        camera_rotate_view(&st_sdl->st_render.st_camera, motion.yrel);
    }

    return SUCCESS;
}

int sdl_handle_wheel(struct sdl_context* st_sdl, SDL_MouseWheelEvent wheel) {
    camera_zoom(&st_sdl->st_render.st_camera, wheel.y);

    return SUCCESS;
}
