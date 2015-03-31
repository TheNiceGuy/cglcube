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
    st_render->window_x = 0;
    st_render->window_y = 0;
    st_render->old_x    = 0;
    st_render->old_y    = 0;
    st_render->fps      = 0;
    st_render->ratio    = 0;

    camera_init(&st_render->st_camera);
    camera_link_render(&st_render->st_camera, st_render);
}

void render_link_sdl(struct render_context* st_render, struct sdl_context* st_sdl) {
    st_render->st_sdl_parent = st_sdl;
}

void render_resize_window(struct render_context* st_render, int x, int y) {
    st_render->window_x = x;
    if(st_render->window_y == 0)
        st_render->window_y = 1;
    else
        st_render->window_y = y;

    st_render->ratio = (double)st_render->window_x/(double)st_render->window_y;
}

void* render_timer(void* st_render_ptr) {
    int time_ticks = 0;
    int time_delay = 0;
    int time_fps   = 0;
    int current_frame = 0;
    struct render_context* st_render = (struct render_context*)st_render_ptr;

    text_init(&st_render->st_overlay_text, 16);
    text_changed(&st_render->st_overlay_text);
    sprintf(st_render->st_overlay_text.text, "%f", st_render->fps);/**/

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

            sprintf(st_render->st_overlay_text.text, "FPS: %f", st_render->fps);
            text_changed(&st_render->st_overlay_text);

            current_frame = 0;
            time_fps = time_ticks;
        }
    }

    SDL_GL_MakeCurrent(st_render->st_sdl_parent->window, NULL);

    text_destroy(&st_render->st_overlay_text);
    return NULL;
}

int render_start(struct render_context* st_render) {
    if(st_render->running)
        return FAILED;

    st_render->running = TRUE;

    pthread_create(&st_render->thread, NULL, &render_timer, st_render);

    return SUCCESS;
}

int render_stop(struct render_context* st_render) {
    if(!st_render->running)
        return FAILED;

    st_render->running = FALSE;
    pthread_join(st_render->thread, NULL);

    return SUCCESS;
}

int render_info(struct render_context* st_render) {
    unsigned char* pixels;
    GLuint texture;

    text_render(&st_render->st_overlay_text);

    pixels = mono_to_RGBA(st_render->st_overlay_text.surface->pixels,
                          st_render->st_overlay_text.surface->w,
                          st_render->st_overlay_text.surface->h);

    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 st_render->st_overlay_text.surface->w,
                 st_render->st_overlay_text.surface->h,
                 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBegin(GL_QUADS);
        glTexCoord2i(0, 1);
        glVertex2f(0, 0);

        glTexCoord2i(1, 1);
        glVertex2f((float)st_render->st_overlay_text.surface->w/
                          st_render->window_x, 0);

        glTexCoord2i(1, 0);
        glVertex2f((float)st_render->st_overlay_text.surface->w/
                          st_render->window_x,
                   (float)st_render->st_overlay_text.surface->h/
                          st_render->window_y);

        glTexCoord2i(0, 0);
        glVertex2f(0, (float)st_render->st_overlay_text.surface->h/
                             st_render->window_y);
    glEnd();
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_BLEND);

    glPixelZoom(1,-1);
    glRasterPos2f(0, (float)st_render->st_overlay_text.surface->h/st_render->window_y);

    free(pixels);

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
