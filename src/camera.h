#ifndef CAMERA_H
#define CAMERA_H

struct camera_context {
    /**
     * Contains a bool defined in config.h that tells if the cube is moving.
     */
    int move;
    /**
     * Contains a bool defined in config.h that tells if the direction of the
     * rotation must be flipped.
     */
    int flip;
    /**
     * Contains the angle of rotation of the scene. It controls the rotation
     * around the Z axis.
     */
    int angle_scene;
    /**
     * Contains the angle of rotation of the camera. It controls the rotation
     * around the Y axis.
     */
    int angle_view;
    /**
     * Contains the distance of the camera.
     */
    double distance;
    /**
     * Contains the X coordinate of the camera.
     */
    double x;
    /**
     * Contains the Y coordinate of the camera.
     */
    double y;
    /**
     * Contains the Z coordinate of the camera.
     */
    double z;
    /**
    * Points to the parent SDL context of the rendering context.
    */
    struct render_context* st_render_parent;
};

/**
 * This function initialises the rendering context with safe defaults.
 *
 * @param st_sdl A pointer to a camera_context structure.
 */
void camera_init(struct camera_context* st_camera);

/**
 * This function links the camera context with its rendering context.
 *
 * @param st_sdl A pointer to a camera_context structure.
 * @param st_sdl A pointer to a render_context structure.
 */
void camera_link_render(struct camera_context* st_camera, struct render_context* st_render);

/**
 * This function resizes the camera viewport. It keeps the aspect ratio when the
 * window is resized.
 *
 * @param st_sdl A pointer to a camera_context structure.
 */
void camera_resize(struct camera_context* st_camera);

/**
 * This function toggles the lock that lets the camera move. It also flips the
 * direction of camera angle in order to have a consistent animation.
 *
 * @param st_sdl A pointer to a camera_context structure.
 */
void camera_toggle_move(struct camera_context* st_camera);

/**
 * This function changes the camera distance.
 *
 * @param st_sdl A pointer to a camera_context structure.
 * @param delta The changement to apply.
 */
void camera_zoom(struct camera_context* st_camera, int delta);

/**
 * This function changes the scene rotation which is equivalent to rotating on
 * the Z axis.
 *
 * @param st_sdl A pointer to a camera_context structure.
 * @param delta The changement to apply.
 */
void camera_rotate_scene(struct camera_context* st_camera, int delta);

/**
 * This function changes the camera rotation which is equivalent to rotating on
 * the Y axis.
 *
 * @param st_sdl A pointer to a camera_context structure.
 * @param delta The changement to apply.
 */
void camera_rotate_view(struct camera_context* st_camera, int delta);

/**
 * This function updates the position of the camera around the Y axis.
 *
 * @param st_sdl A pointer to a camera_context structure.
 */
void camera_update_position(struct camera_context* st_camera);

/**
 * This function places the camera to its parameters.
 *
 * @param st_sdl A pointer to a camera_context structure.
 * @return An error code defined in config.h.
 */
int camera_update(struct camera_context* st_camera);

/** @file */
#endif
