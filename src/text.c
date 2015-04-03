#include "config.h"
#include "text.h"
#include "std.h"

void text_change(struct text* st_text, char* text) {
    st_text->text = text;

    text_render(st_text);
    text_bind_texture(st_text);
}

int text_init(struct text* st_text) {
    st_text->font = TTF_OpenFont(DEFAULT_FONT, DEFAULT_SIZE);
    if(st_text->font == NULL) {
        printf("%s", TTF_GetError());
        return FAIL;
    }

    TTF_SetFontStyle(st_text->font, TTF_STYLE_NORMAL);
    TTF_SetFontHinting(st_text->font, TTF_HINTING_MONO);

    st_text->text = NULL;
    st_text->color.r = 255;
    st_text->color.g = 255;
    st_text->color.b = 255;
    st_text->color.a = 255;
    st_text->texture = 0;
    st_text->st_loc.x = 0;
    st_text->st_loc.y = 0;

    /*
     * In order to prevent free from being called with an invalid pointer, we
     * allocate a placeholder into the pointer.
     */
    st_text->pixels = malloc(16);
    st_text->surface = SDL_CreateRGBSurface(0,0,0,32,0,0,0,0);

    return SUCCESS;
}

int text_destroy(struct text* st_text) {
    free(st_text->pixels);

    TTF_CloseFont(st_text->font);
    SDL_FreeSurface(st_text->surface);

    return SUCCESS;
}

int text_render(struct text* st_text) {
    free(st_text->pixels);
    SDL_FreeSurface(st_text->surface);

    st_text->surface = TTF_RenderText_Solid(st_text->font,
                            st_text->text, st_text->color);
    if(st_text->surface == NULL) {
        printf("%s", TTF_GetError());
        return FAIL;
    }

    st_text->pixels = mono_to_RGBA(st_text->surface->pixels,
                                   st_text->surface->w,
                                   st_text->surface->h);

    return SUCCESS;
}

int text_bind_texture(struct text* st_text) {
    glGenTextures(1, &st_text->texture);
    glBindTexture(GL_TEXTURE_2D, st_text->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 st_text->surface->w,
                 st_text->surface->h,
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 st_text->pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return SUCCESS;
}

int text_draw(struct text* st_text, struct dimension* st_screen) {
    glBindTexture(GL_TEXTURE_2D, st_text->texture);

    glBegin(GL_QUADS);
        glTexCoord2i(0, 1);
        glVertex2f((float)st_text->st_loc.x/
                          st_screen->w,
                   (float)st_text->st_loc.y/
                          st_screen->h);

        glTexCoord2i(1, 1);
        glVertex2f((float)(st_text->st_loc.x+st_text->surface->w)/
                           st_screen->w,
                   (float) st_text->st_loc.y/
                           st_screen->h);

        glTexCoord2i(1, 0);
        glVertex2f((float)(st_text->st_loc.x+st_text->surface->w)/
                           st_screen->w,
                   (float)(st_text->st_loc.y+st_text->surface->h)/
                           st_screen->h);

        glTexCoord2i(0, 0);
        glVertex2f((float)st_text->st_loc.x/
                          st_screen->w,
                   (float)(st_text->st_loc.y+st_text->surface->h)/
                           st_screen->h);
    glEnd();

    return SUCCESS;
}
