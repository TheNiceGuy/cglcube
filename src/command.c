#include <stdio.h>

#include "config.h"
#include "command.h"

void command_init(struct command_context* st_cmd) {
    st_cmd->text   = malloc(CMD_BUFFER);
    st_cmd->cursor = 0;
}

void command_free(struct command_context* st_cmd) {
    free(st_cmd->text);
}

void command_clear(struct command_context* st_cmd) {
    st_cmd->cursor = 0;
    st_cmd->text[st_cmd->cursor] = '\0';
}

int command_append(struct command_context* st_cmd, char* input) {
    int i = 0;

    while(input[i] != '\0') {
        st_cmd->text[st_cmd->cursor] = input[i];

        if(st_cmd->cursor < CMD_BUFFER-1)
            st_cmd->cursor++;
        i++;
    }
    st_cmd->text[st_cmd->cursor] = '\0';

    printf("%s\n", st_cmd->text);

    return SUCCESS;
}

int command_backspace(struct command_context* st_cmd) {
    if(st_cmd->cursor > 0)
        st_cmd->cursor--;
    st_cmd->text[st_cmd->cursor] = '\0';

    printf("%s\n", st_cmd->text);

    return SUCCESS;
}

int command_execute(struct command_context* st_cmd) {
    command_clear(st_cmd);

    return SUCCESS;
}
