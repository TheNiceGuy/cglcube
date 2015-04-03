#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>

#define CMD_BUFFER 64

struct command_context {
    char* text;
    int cursor;
};

void command_init(struct command_context* st_cmd);
void command_free(struct command_context* st_cmd);
void command_clear(struct command_context* st_cmd);
int command_append(struct command_context* st_cmd, char* input);
int command_backspace(struct command_context* st_cmd);
int command_execute(struct command_context* st_cmd);

#endif
