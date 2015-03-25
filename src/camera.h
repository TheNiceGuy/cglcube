#ifndef CAMERA_H
#define CAMERA_H

struct camera_context {
    double x, y, z;
    double angle;

    /**
    * Points to the parent SDL context of the rendering context.
    */
    struct render_context* st_render_parent;
};

void camera_init(struct camera_context* st_camera);
/**
 * This function links the camera context with its rendering context.
 *
 * @param st_sdl A pointer to a camera_context structure.
 * @param st_sdl A pointer to a render_context structure.
 */
void  camera_link_render(struct camera_context* st_camera, struct render_context* st_render);
void camera_resize(struct camera_context* st_camera);
int camera_update(struct camera_context* st_camera);

#endif
