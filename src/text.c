#include "config.h"
#include "text.h"
#include "std.h"

int text_init(struct text* st_text) {
    st_text->font = TTF_OpenFont(DEFAULT_FONT, DEFAULT_SIZE);
    if(st_text->font == NULL) {
        printf("%s", TTF_GetError());
        return FAILED;
    }

    TTF_SetFontStyle(st_text->font, TTF_STYLE_NORMAL);
    TTF_SetFontHinting(st_text->font, TTF_HINTING_MONO);

    st_text->text = NULL;
    st_text->color.r = 255;
    st_text->color.g = 255;
    st_text->color.b = 255;
    st_text->color.a = 255;
    st_text->updated = FALSE;

    /*
     * In order to prevent free from being called with a valid pointer, we
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
    if(!st_text->updated) {
        return FAILED;
    }

    free(st_text->pixels);
    SDL_FreeSurface(st_text->surface);

    st_text->surface = TTF_RenderText_Solid(st_text->font,
                            st_text->text, st_text->color);
    if(st_text->surface == NULL) {
        printf("%s", TTF_GetError());
        return FAILED;
    }

    st_text->pixels = mono_to_RGBA(st_text->surface->pixels,
                                   st_text->surface->w,
                                   st_text->surface->h);
    st_text->updated = FALSE;

    return SUCCESS;
}

void text_change(struct text* st_text, char* text) {
    st_text->text = text;
    st_text->updated = TRUE;
}
