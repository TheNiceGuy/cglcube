#include <unistd.h>
#include "render.h"
#include "sdl.h"
#include "draw.h"

void  render_init(struct render_context* st_render, int x, int y) {
    st_render->running  = FALSE;
    st_render->window_x = x;
    st_render->window_y = y;
}

void render_link_sdl(struct render_context* st_render, struct sdl_context* st_sdl) {
    st_render->st_sdl_parent = st_sdl;
}

void* render_timer(void* st_render_ptr) {
    int time_ticks = 0;
    int time_delay = 0;
    struct render_context* st_render = (struct render_context*)st_render_ptr;

    sdl_create_opengl(st_render->st_sdl_parent);
    sdl_opengl_version(st_render->st_sdl_parent);

    while(st_render->running == TRUE) {
        time_ticks = SDL_GetTicks();

        if(time_ticks - time_delay > (1000/60)) {
            pthread_mutex_lock(&st_render->thread_mutex);
            render_window(st_render);
            pthread_mutex_unlock(&st_render->thread_mutex);

            time_delay = time_ticks;
        }
    }

    sdl_free(st_render->st_sdl_parent);
    return NULL;
}

int render_start(struct render_context* st_render) {
    st_render->running = TRUE;
    st_render->thread_mutex = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;

    pthread_create(&st_render->thread, NULL, &render_timer, st_render);
    sleep(1);

    pthread_mutex_lock(&st_render->thread_mutex);
    pthread_mutex_unlock(&st_render->thread_mutex);

    return SUCCESS;
}

int render_stop(struct render_context* st_render) {
    st_render->running = FALSE;
    pthread_join(st_render->thread, NULL);

    return SUCCESS;
}

int render_window(struct render_context* st_render) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(0.1, 0.39, 0.88, 1.0);
    glLoadIdentity();

    draw_2Dgrid(20, 1);

    glFlush();
    SDL_GL_SwapWindow(st_render->st_sdl_parent->window);

    return SUCCESS;
}
