#ifndef TEXT_H
#define TEXT_H

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <GL/gl.h>
#include "std.h"

#define DEFAULT_SIZE 12
#define DEFAULT_FONT "./font/DejaVuSans.ttf"
#define BUFFER 64

enum position {
    TOP    = 1 << 0,
    BOTTOM = 1 << 1,
    LEFT   = 1 << 2,
    RIGHT  = 1 << 3
};

/**
 * This data structure is used for rendering text in SDL.
 */
struct text {
    /**
     * Contains a pointer to the text buffer.
     */
    char* text;
    /**
     * Contains the size of the text buffer.
     */
    int size;
    /**
     * Contains the location of the text on the screen.
     */
    struct point st_loc;
    /**
     * Contains the relative position of the text.
     */
    enum position flags_pos;
    /**
     * Contains the texture ID for the text.
     */
    GLuint texture;
    /**
     * Contains the color of the text.
     */
    SDL_Color color;
    /**
     * Contains the surface returned by TTF_RenderText*.
     */
    SDL_Surface* surface;
    SDL_mutex*  mutex;
    /**
     * Contains the font for rendering text.
     */
    TTF_Font* font;
};

/**
 * This tells the structure that it has been updated and
 * needs to be re-rendered.
 *
 * @param st_text A pointer to a text structure.
 */
void text_change(struct text* st_text, char* text);

void text_move(struct text* st_text, struct dimension* st_screen);

/**
 * This function initialises the text structure.
 *
 * @param st_text A pointer to a text structure.
 * @param size The size of the text buffer.
 * @return An error code defined in config.h.
 */
int text_init(struct text* st_text, enum position flags_pos);

/**
 * This function destroys and frees the text structure.
 *
 * @param st_text A pointer to a text structure.
 * @return An error code defined in config.h.
 */
int text_destroy(struct text* st_text);

/**
 * This function renders the text into the surface.
 *
 * @param st_text A pointer to a text structure.
 * @return An error code defined in config.h.
 */
int text_render(struct text* st_text);

int text_bind_texture(struct text* st_text);

int text_draw(struct text* st_text, struct dimension* st_screen);

#endif
