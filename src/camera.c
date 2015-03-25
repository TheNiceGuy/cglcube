#include <GL/glu.h>
#include "config.h"
#include "render.h"
#include "camera.h"

void camera_init(struct camera_context* st_camera) {
    st_camera->x = DEFAULT_X;
    st_camera->y = DEFAULT_Y;
    st_camera->z = DEFAULT_Z;
}

void camera_link_render(struct camera_context* st_camera, struct render_context* st_render) {
    st_camera->st_render_parent = st_render;
}

void camera_resize(struct camera_context* st_camera) {
    glViewport(0, 0,
               st_camera->st_render_parent->window_x,
               st_camera->st_render_parent->window_y);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, st_camera->st_render_parent->ratio, 0.1, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();/**/
}

int camera_update(struct camera_context* st_camera) {
    gluPerspective(45.0, 1, 0.1, 100.0);
    gluLookAt(1, 1, 1, 0, 0, 0, 0, 0, 1);

    return SUCCESS;
}
