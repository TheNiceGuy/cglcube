#ifndef RENDER_H
#define RENDER_H

#include <pthread.h>
#include "config.h"

struct render_context {
	int running;
    int window_x, window_y;

	struct sdl_context* st_sdl_parent;

	pthread_t thread;
	pthread_mutex_t thread_mutex;
};

void  render_init(struct render_context* st_render, int x, int y);
void  render_link_sdl(struct render_context* st_render, struct sdl_context* st_sdl);
void* render_timer(void* st_render_ptr);
int render_start(struct render_context* st_render);
int render_stop(struct render_context* st_render);
int render_window(struct render_context* st_render);

#endif
