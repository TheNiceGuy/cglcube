#if defined(__LINUX__)
    #include <unistd.h>
#elif defined(__WIN32__)
    #include <windows.h>
#endif
#include "config.h"
#include "render.h"
#include "sdl.h"
#include "camera.h"
#include "draw.h"
#include "std.h"

void render_init(struct render_context* st_render) {
    st_render->running  = FALSE;
    st_render->fps      = 0;
    st_render->ratio    = 0;
    st_render->st_screen.w = 0;
    st_render->st_screen.h = 0;
    st_render->st_screen_old.w = 0;
    st_render->st_screen_old.h = 0;

    camera_init(&st_render->st_camera);
    camera_link_render(&st_render->st_camera, st_render);
}

void render_link_sdl(struct render_context* st_render, struct sdl_context* st_sdl) {
    st_render->st_sdl_parent = st_sdl;
}

void render_resize_window(struct render_context* st_render, int x, int y) {
    st_render->st_screen.w = x;
    if(st_render->st_screen.h == 0)
        st_render->st_screen.h = 1;
    else
        st_render->st_screen.h = y;

    st_render->ratio = (double)st_render->st_screen.w/
                       (double)st_render->st_screen.h;
}

int render_timer(void* st_render_ptr) {
    char* fps;
    int time_ticks = 0;
    int time_delay = 0;
    int time_fps   = 0;
    int current_frame = 0;
    struct render_context* st_render = (struct render_context*)st_render_ptr;

    fps = malloc(16);

    sprintf(fps, "FPS: %f", st_render->fps);
    text_init(&st_render->st_text_fps);
    text_change(&st_render->st_text_fps, fps);

    SDL_GL_MakeCurrent(st_render->st_sdl_parent->window,
                       st_render->st_sdl_parent->render_glcontext);

    while(st_render->running) {
        time_ticks = SDL_GetTicks();

        /*
         * Draw the image.
         */
        if(time_ticks-time_delay > 1000/120) {
            render_window(st_render);

            current_frame++;
            time_delay = time_ticks;
        }

        /*
         * Update the fps after one second.
         */
        if(time_ticks-time_fps > 1000) {
            st_render->fps = (float)current_frame/(time_ticks-time_fps)*1000;

            sprintf(fps, "FPS: %f", st_render->fps);
            text_change(&st_render->st_text_fps, fps);

            current_frame = 0;
            time_fps = time_ticks;
        }
    }

    SDL_GL_MakeCurrent(st_render->st_sdl_parent->window, NULL);

    free(fps);
    text_destroy(&st_render->st_text_fps);
    return SUCCESS;
}

int render_start(struct render_context* st_render) {
    if(st_render->running)
        return FAILED;

    st_render->running = TRUE;
    st_render->thread = SDL_CreateThread(render_timer, "RenderThread", st_render);

    return SUCCESS;
}

int render_stop(struct render_context* st_render) {
    if(!st_render->running)
        return FAILED;

    st_render->running = FALSE;
    SDL_WaitThread(st_render->thread, NULL);

    return SUCCESS;
}

int render_info(struct render_context* st_render) {
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    /*
     * Render every text on the screen.
     */
    text_draw(&st_render->st_text_fps, &st_render->st_screen);

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    return SUCCESS;
}

int render_overlay(struct render_context* st_render) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,1,0,1,0,1);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    render_info(st_render);

    return SUCCESS;
}

int render_window(struct render_context* st_render) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.39, 0.88, 0);

    camera_update(&st_render->st_camera);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    draw_2Dgrid(10, 0.2);

    render_overlay(st_render);

    glFlush();
    SDL_GL_SwapWindow(st_render->st_sdl_parent->window);

    return SUCCESS;
}
