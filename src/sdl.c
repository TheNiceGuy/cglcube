#include "config.h"
#include "sdl.h"

void sdl_version(struct sdl_context* st_sdl) {
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

void sdl_ttf_version(struct sdl_context* st_sdl) {
    SDL_version        compile_version;
    SDL_version const *link_version = TTF_Linked_Version();

    SDL_TTF_VERSION(&compile_version);

    printf("TTF compiled version:\t %d.%d.%d\n",
            compile_version.major,
            compile_version.minor,
            compile_version.patch);
    printf("TTF linked version:\t %d.%d.%d\n",
            link_version->major,
            link_version->minor,
            link_version->patch);
}

void sdl_init(struct sdl_context* st_sdl) {
    st_sdl->running      = FALSE;
    st_sdl->command_mode = FALSE;
    st_sdl->fullscreen   = FALSE;

    render_init(&st_sdl->st_render);
    render_link_sdl(&st_sdl->st_render, st_sdl);

    command_init(&st_sdl->st_cmd);
    command_link_sdl(&st_sdl->st_cmd, st_sdl);
}

int sdl_start(struct sdl_context* st_sdl) {
    if(st_sdl->running)
        return FAIL;

    if(SDL_Init(SDL_INIT_VIDEO) != SUCCESS) {
        printf("Unable to initialize SDL: %s\n", SDL_GetError());
        exit(FAIL);
    }
    sdl_version(st_sdl);

    if(TTF_Init() != SUCCESS) {
        printf("TTF_Init: %s\n", TTF_GetError());
        exit(FAIL);
    }
    sdl_ttf_version(st_sdl);

    sdl_create_opengl(st_sdl);
    sdl_opengl_version(st_sdl);

    render_start(&st_sdl->st_render);

    st_sdl->running = TRUE;
    return SUCCESS;
}

int sdl_stop(struct sdl_context* st_sdl) {
    if(!st_sdl->running)
        return FAIL;

    render_stop(&st_sdl->st_render);
    command_free(&st_sdl->st_cmd);
    sdl_free(st_sdl);

    st_sdl->running = FALSE;
    return SUCCESS;
}

int sdl_create_opengl(struct sdl_context* st_sdl) {
    sdl_resolution_start(st_sdl);

    SDL_GL_SetAttribute(SDL_GL_SHARE_WITH_CURRENT_CONTEXT, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 1);
    SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 4);
    SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
    st_sdl->window = SDL_CreateWindow(NAME, SDL_WINDOWPOS_CENTERED,
                                      SDL_WINDOWPOS_CENTERED,
                                      st_sdl->st_render.st_screen.w,
                                      st_sdl->st_render.st_screen.h,
                                      SDL_WINDOW_OPENGL);
    if(st_sdl->window == NULL) {
        printf("Can't create SDL window: %s\n", SDL_GetError());
        return FAIL;
    }

    st_sdl->render_glcontext = SDL_GL_CreateContext(st_sdl->window);
    st_sdl->window_glcontext = SDL_GL_CreateContext(st_sdl->window);

    SDL_GL_SetSwapInterval(0);
    SDL_GL_MakeCurrent(st_sdl->window, st_sdl->window_glcontext);

    return SUCCESS;
}

int sdl_free(struct sdl_context* st_sdl) {
    SDL_GL_DeleteContext(st_sdl->render_glcontext);
    SDL_GL_DeleteContext(st_sdl->window_glcontext);
    SDL_DestroyWindow(st_sdl->window);
    SDL_Quit();

    return SUCCESS;
}

int sdl_resolution_start(struct sdl_context* st_sdl) {
    int index, display_index, total_index;
    int area = 0;
    SDL_DisplayMode mode;

    display_index = SDL_GetNumVideoDisplays()-1;
    total_index   = SDL_GetNumDisplayModes(display_index);

    for(index = 0; index < total_index; index++) {
        SDL_GetDisplayMode(display_index, index, &mode);

        if(mode.w*mode.h < area || area == 0) {
            st_sdl->st_render.st_screen.w = mode.w;
            st_sdl->st_render.st_screen.h = mode.h;
            area = mode.w*mode.h;
        }
    }

    render_resize_window(&st_sdl->st_render,
                          st_sdl->st_render.st_screen.w,
                          st_sdl->st_render.st_screen.h);

    return SUCCESS;
}

int sdl_resolution_increase(struct sdl_context* st_sdl) {
    int index, display_index, total_index;
    int area = st_sdl->st_render.st_screen.w*
               st_sdl->st_render.st_screen.h;
    SDL_DisplayMode mode;

    display_index = SDL_GetNumVideoDisplays()-1;
    total_index   = SDL_GetNumDisplayModes(display_index)-1;

    for(index = total_index; index >= 0 ; index--) {
        SDL_GetDisplayMode(display_index, index, &mode);

        if(mode.w*mode.h > area) {
            st_sdl->st_render.st_screen.w = mode.w;
            st_sdl->st_render.st_screen.h = mode.h;
            break;
        }
    }

    SDL_LockMutex(st_sdl->st_render.mutex);
    render_resize_window(&st_sdl->st_render,
                          st_sdl->st_render.st_screen.w,
                          st_sdl->st_render.st_screen.h);
    SDL_SetWindowSize(st_sdl->window, mode.w, mode.h);
    SDL_UnlockMutex(st_sdl->st_render.mutex);

    return SUCCESS;
}

int sdl_resolution_decrease(struct sdl_context* st_sdl) {
    int index, display_index, total_index;
    int area = st_sdl->st_render.st_screen.w*
               st_sdl->st_render.st_screen.h;
    SDL_DisplayMode mode;

    display_index = SDL_GetNumVideoDisplays()-1;
    total_index   = SDL_GetNumDisplayModes(display_index);

    for(index = 0; index < total_index ; index++) {
        SDL_GetDisplayMode(display_index, index, &mode);

        if(mode.w*mode.h < area) {
            st_sdl->st_render.st_screen.w = mode.w;
            st_sdl->st_render.st_screen.h = mode.h;
            break;
        }
    }
    /*
     * Disable fullscreen if it is enabled.
     */
    if(st_sdl->fullscreen) {
        st_sdl->fullscreen = FALSE;
    }

    SDL_LockMutex(st_sdl->st_render.mutex);
    render_resize_window(&st_sdl->st_render,
                          st_sdl->st_render.st_screen.w,
                          st_sdl->st_render.st_screen.h);
    SDL_SetWindowSize(st_sdl->window, mode.w, mode.h);
    SDL_UnlockMutex(st_sdl->st_render.mutex);

    return SUCCESS;
}

int sdl_resolution_set(struct sdl_context* st_sdl, int w, int h) {
    SDL_LockMutex(st_sdl->st_render.mutex);
    render_resize_window(&st_sdl->st_render, w, h);
    SDL_SetWindowSize(st_sdl->window, w, h);
    SDL_UnlockMutex(st_sdl->st_render.mutex);

    return SUCCESS;
}

int sdl_fullscreen(struct sdl_context* st_sdl) {
    SDL_Rect screen;

    SDL_GetDisplayBounds(0, &screen);

    SDL_LockMutex(st_sdl->st_render.mutex);
    if(!st_sdl->fullscreen) {
        /*
         * Save the old resolution if the user disable fullscreen
         * later in the execution.
         */
        st_sdl->st_render.st_screen_old.w = st_sdl->st_render.st_screen.w;
        st_sdl->st_render.st_screen_old.h = st_sdl->st_render.st_screen.h;

        render_resize_window(&st_sdl->st_render, screen.w, screen.h);
        SDL_SetWindowSize(st_sdl->window, screen.w, screen.h);
        st_sdl->fullscreen = TRUE;
    } else {
        SDL_SetWindowSize(st_sdl->window,
                          st_sdl->st_render.st_screen_old.w,
                          st_sdl->st_render.st_screen_old.h);
        render_resize_window(&st_sdl->st_render,
                              st_sdl->st_render.st_screen_old.w,
                              st_sdl->st_render.st_screen_old.h);
        st_sdl->fullscreen = FALSE;
    }
    SDL_UnlockMutex(st_sdl->st_render.mutex);

    return SUCCESS;
}

int sdl_handle_event(struct sdl_context* st_sdl) {
    if(!st_sdl->running)
        return FAIL;

    while (st_sdl->running) {
        SDL_WaitEvent(&st_sdl->event);

        switch(st_sdl->event.type) {
        case SDL_QUIT:
            sdl_stop(st_sdl);
            break;

        case SDL_KEYDOWN:
            if(st_sdl->command_mode)
                sdl_handle_key_cmd(st_sdl, st_sdl->event.key.keysym);
            else
                sdl_handle_key(st_sdl, st_sdl->event.key.keysym);
            break;

        case SDL_TEXTINPUT:
            if(st_sdl->command_mode) {
                if(!command_append(&st_sdl->st_cmd, st_sdl->event.text.text))
                    text_change(&st_sdl->st_render.st_text_cmd,
                                 st_sdl->st_cmd.text);
            }
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

int sdl_handle_key(struct sdl_context* st_sdl, SDL_Keysym key) {
    switch(key.sym) {
    case SDLK_ESCAPE:
    case SDLK_q:
        sdl_stop(st_sdl);
        break;
    case SDLK_f:
        sdl_fullscreen(st_sdl);
        break;
    case SDLK_PAGEUP:
        sdl_resolution_increase(st_sdl);
        break;
    case SDLK_PAGEDOWN:
        sdl_resolution_decrease(st_sdl);
        break;
    case SDLK_SEMICOLON:
        if(key.mod & KMOD_SHIFT) {
            st_sdl->command_mode = TRUE;
            if(!command_append(&st_sdl->st_cmd, ":\0"))
                text_change(&st_sdl->st_render.st_text_cmd, st_sdl->st_cmd.text);
        }
        break;
    }

    return SUCCESS;
}

int sdl_handle_key_cmd(struct sdl_context* st_sdl, SDL_Keysym key) {
    switch(key.sym) {
    case SDLK_ESCAPE:
        st_sdl->command_mode = FALSE;
        command_clear(&st_sdl->st_cmd);
        text_change(&st_sdl->st_render.st_text_cmd, st_sdl->st_cmd.text);
        break;
    case SDLK_BACKSPACE:
        command_backspace(&st_sdl->st_cmd);
        text_change(&st_sdl->st_render.st_text_cmd, st_sdl->st_cmd.text);
        break;
    case SDLK_RETURN:
        st_sdl->command_mode = FALSE;
        command_execute(&st_sdl->st_cmd);
        text_change(&st_sdl->st_render.st_text_cmd, st_sdl->st_cmd.text);
        break;
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
