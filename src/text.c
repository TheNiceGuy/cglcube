#include "config.h"
#include "text.h"
#include "std.h"

void text_change(struct text* st_text, char* text) {
    if(!strcmp(st_text->text, text)) {
        return;
    }
    strcpy(st_text->text, text);

    SDL_LockMutex(st_text->mutex);
    text_render(st_text);
    text_bind_texture(st_text);
    SDL_UnlockMutex(st_text->mutex);
}

void text_move(struct text* st_text, struct dimension* st_screen) {
    if(st_text->flags_pos & RIGHT) {
        st_text->st_loc.x = st_screen->w-st_text->surface->w;
    } else if(st_text->flags_pos & LEFT) {
        st_text->st_loc.x = 0;
    } else {
        st_text->st_loc.x = 0;
    }

    if(st_text->flags_pos & TOP) {
        st_text->st_loc.y = st_screen->h-st_text->surface->h;
    } else if(st_text->flags_pos & BOTTOM) {
        st_text->st_loc.y = 0;
    } else {
        st_text->st_loc.y = 0;
    }
}

int text_init(struct text* st_text, enum position flags_pos) {
    st_text->font = TTF_OpenFont(DEFAULT_FONT, DEFAULT_SIZE);
    if(st_text->font == NULL) {
        printf("%s", TTF_GetError());
        return FAIL;
    }

/*    TTF_SetFontStyle(st_text->font, TTF_STYLE_NORMAL);
    TTF_SetFontHinting(st_text->font, TTF_HINTING_MONO);
    TTF_SetFontKerning(st_text->font, 0);
    TTF_SetFontOutline(st_text->font, 0);*/

    st_text->text = malloc(BUFFER);
    st_text->color.r = 0;
    st_text->color.g = 0;
    st_text->color.b = 0;
    st_text->color.a = 0;
    st_text->texture = 0;
    st_text->st_loc.x = 0;
    st_text->st_loc.y = 0;
    st_text->flags_pos = flags_pos;
    st_text->mutex = SDL_CreateMutex();

    memset(st_text->text, '\0', BUFFER);
    /*
     * In order to prevent free from being called with an invalid pointer, we
     * allocate a placeholder into the pointer.
     */
    st_text->surface = SDL_CreateRGBSurface(0,0,0,32,0,0,0,0);

    return SUCCESS;
}

int text_destroy(struct text* st_text) {
    free(st_text->text);

    TTF_CloseFont(st_text->font);
    SDL_FreeSurface(st_text->surface);
    SDL_DestroyMutex(st_text->mutex);

    return SUCCESS;
}

int text_render(struct text* st_text) {
    SDL_FreeSurface(st_text->surface);

    st_text->surface = TTF_RenderText_Blended(st_text->font,
                               st_text->text, st_text->color);
    if(st_text->surface == NULL) {
        printf("%s", TTF_GetError());
        return FAIL;
    }

    return SUCCESS;
}

int text_bind_texture(struct text* st_text) {
    glGenTextures(1, &st_text->texture);
    glBindTexture(GL_TEXTURE_2D, st_text->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
                 st_text->surface->w,
                 st_text->surface->h,
                 0, GL_RGBA, GL_UNSIGNED_BYTE,
                 st_text->surface->pixels);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    return SUCCESS;
}

int text_draw(struct text* st_text, struct dimension* st_screen) {
    text_move(st_text, st_screen);

    glBindTexture(GL_TEXTURE_2D, st_text->texture);

    SDL_LockMutex(st_text->mutex);
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
    SDL_UnlockMutex(st_text->mutex);

    return SUCCESS;
}
