#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>
#include "text.h"

#define CMD_BUFFER 64
#define ERR_BUFFER 64

struct command_context {
    char* text;
    char* error;
    int cursor;

    struct sdl_context* st_sdl_parent;
    struct text* st_text;
};

void command_init(struct command_context* st_cmd);
void command_link_sdl(struct command_context* st_cmd, struct sdl_context* st_sdl);
void command_link_text(struct command_context* st_cmd, struct text* st_text);
void command_free(struct command_context* st_cmd);
void command_clear(struct command_context* st_cmd);
void command_cancel(struct command_context* st_cmd);
void command_set_error(struct command_context* st_cmd, char* error);
int command_append(struct command_context* st_cmd, char* input);
int command_backspace(struct command_context* st_cmd);
int command_execute(struct command_context* st_cmd);
int command_handle_set(struct command_context* st_cmd, char** argv, int argc);

#endif
