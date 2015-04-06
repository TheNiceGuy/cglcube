#include <stdio.h>
#include "config.h"
#include "command.h"
#include "std.h"
#include "sdl.h"

void command_init(struct command_context* st_cmd) {
    st_cmd->text   = malloc(CMD_BUFFER);
    st_cmd->cursor = 0;
}

void command_link_sdl(struct command_context* st_cmd, struct sdl_context* st_sdl) {
    st_cmd->st_sdl_parent = st_sdl;
}

void command_free(struct command_context* st_cmd) {
    free(st_cmd->text);
}

void command_clear(struct command_context* st_cmd) {
    st_cmd->cursor = 0;
    st_cmd->text[st_cmd->cursor+0] = ' ';
    st_cmd->text[st_cmd->cursor+1] = '\0';
}

int command_append(struct command_context* st_cmd, char* input) {
    int i = 0;

    if(*(input) == ':' && st_cmd->cursor != 0)
        return FAIL;

    while(input[i] != '\0') {
        st_cmd->text[st_cmd->cursor] = input[i];

        if(st_cmd->cursor < CMD_BUFFER-1)
            st_cmd->cursor++;
        i++;
    }
    st_cmd->text[st_cmd->cursor] = '\0';

    return SUCCESS;
}

int command_backspace(struct command_context* st_cmd) {
    if(st_cmd->cursor > 1)
        st_cmd->cursor--;

    if(st_cmd->cursor == 0) {
        st_cmd->text[st_cmd->cursor  ] = ' ';
        st_cmd->text[st_cmd->cursor+1] = '\0';

        return SUCCESS;
    }

    st_cmd->text[st_cmd->cursor  ] = '\0';

    return SUCCESS;
}

int command_execute(struct command_context* st_cmd) {
    char** argv;
    int    argc, i;

    split_argument(&argv, &argc, st_cmd->text+1);

    /*
     * Proof of concept
     */
    for(i = 0; i < argc; i++) {
        if(!strcmp(argv[i], "set")) {
            if(!strcmp(argv[i+1], "resolution")) {
                printf("%d %d\n", atoi(argv[i+2]), atoi(argv[i+3]));
                sdl_resolution_set(st_cmd->st_sdl_parent, atoi(argv[i+2]), atoi(argv[i+3]));
            }
        }
    }


    for(i = 0; i < argc; i++)
        free(argv[i]);
    free(argv);

    command_clear(st_cmd);

    return SUCCESS;
}
