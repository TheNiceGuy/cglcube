#include <math.h>
#include <GL/glu.h>
#include "config.h"
#include "render.h"
#include "camera.h"

void camera_init(struct camera_context* st_camera) {
    st_camera->move = FALSE;
    st_camera->flip = FALSE;
    st_camera->angle_scene = 45;
    st_camera->angle_view  = 45;
    st_camera->distance = 5;

    camera_update_position(st_camera);
}

void camera_link_render(struct camera_context* st_camera, struct render_context* st_render) {
    st_camera->st_render_parent = st_render;
}

void camera_resize(struct camera_context* st_camera) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0,
              (GLsizei)st_camera->st_render_parent->st_screen.w,
              (GLsizei)st_camera->st_render_parent->st_screen.h);
    gluPerspective(45.0, st_camera->st_render_parent->ratio, 0.1, 100.0);
}

void camera_toggle_move(struct camera_context* st_camera) {
    if(st_camera->move) {
        if((st_camera->angle_view > 90 && st_camera->angle_view < 270)
        || (st_camera->angle_view <-90 && st_camera->angle_view >-270))
        {
            st_camera->flip = TRUE;
        } else {
            st_camera->flip = FALSE;
        }
        st_camera->move = FALSE;
    } else
        st_camera->move = TRUE;
}

void camera_zoom(struct camera_context* st_camera, int delta) {
    st_camera->distance = st_camera->distance - delta;

    if(st_camera->distance < ZOOM_MIN)
        st_camera->distance = ZOOM_MIN;
    else if(st_camera->distance > ZOOM_MAX)
        st_camera->distance = ZOOM_MAX;

    camera_update_position(st_camera);
}

void camera_rotate_scene(struct camera_context* st_camera, int delta) {
    if(st_camera->flip)
        st_camera->angle_scene = st_camera->angle_scene - delta;
    else
        st_camera->angle_scene = st_camera->angle_scene + delta;

    if(st_camera->angle_scene <= -360)
        st_camera->angle_scene = st_camera->angle_scene + 360;
    else if(st_camera->angle_scene >= 360)
        st_camera->angle_scene = st_camera->angle_scene - 360;
}

void camera_rotate_view(struct camera_context* st_camera, int delta) {
    st_camera->angle_view = st_camera->angle_view + delta;

    if(st_camera->angle_view <= -360)
        st_camera->angle_view = st_camera->angle_view + 360;
    else if(st_camera->angle_view >= 360)
        st_camera->angle_view = st_camera->angle_view - 360;

    camera_update_position(st_camera);
}

void camera_update_position(struct camera_context* st_camera) {
    st_camera->x = st_camera->distance*
                   cos((st_camera->angle_view*M_PI)/180);
    st_camera->y = 0;
    st_camera->z = st_camera->distance*
                   sin((st_camera->angle_view*M_PI)/180);

}

int camera_update(struct camera_context* st_camera) {
    camera_resize(st_camera);

    if((st_camera->angle_view <= 90 && st_camera->angle_view >= -90)
    || (st_camera->angle_view > 270 || st_camera->angle_view < -270))
    {
        gluLookAt(st_camera->x,
                  st_camera->y,
                  st_camera->z,
                  0, 0, 0, 0, 0, 1);
    } else {
        gluLookAt(st_camera->x,
                  st_camera->y,
                  st_camera->z,
                  0, 0, 0, 0, 0,-1);
    }

    glRotated(st_camera->angle_scene, 0, 0, 1);

    return SUCCESS;
}
