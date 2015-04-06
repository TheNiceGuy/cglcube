#include <stdio.h>
#include <ctype.h>
#include "config.h"
#include "command.h"
#include "std.h"
#include "sdl.h"

void command_init(struct command_context* st_cmd) {
    st_cmd->text   = malloc(CMD_BUFFER);
    st_cmd->error  = malloc(ERR_BUFFER);
    st_cmd->cursor = 0;
}

void command_link_sdl(struct command_context* st_cmd, struct sdl_context* st_sdl) {
    st_cmd->st_sdl_parent = st_sdl;
}

void command_link_text(struct command_context* st_cmd, struct text* st_text) {
    st_cmd->st_text = st_text;
}

void command_free(struct command_context* st_cmd) {
    free(st_cmd->text);
    free(st_cmd->error);
}

void command_clear(struct command_context* st_cmd) {
    st_cmd->cursor = 0;
    st_cmd->text[st_cmd->cursor+0] = ' ';
    st_cmd->text[st_cmd->cursor+1] = '\0';
}

void command_cancel(struct command_context* st_cmd) {
    command_clear(st_cmd);
    text_change(st_cmd->st_text, st_cmd->text);
}

void command_set_error(struct command_context* st_cmd, char* error) {
    strncpy(st_cmd->error, error, ERR_BUFFER);
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
    text_change(st_cmd->st_text, st_cmd->text);

    return SUCCESS;
}

int command_execute(struct command_context* st_cmd) {
    char** argv;
    int    argc, i;

    parse_argument(&argv, &argc, st_cmd->text+1);
    command_clear(st_cmd);

    if(argc == 0) {
        text_change(st_cmd->st_text, st_cmd->text);
        free(argv);
        return SUCCESS;
    }

    if(strcmp(argv[0], "set") == 0 && argc > 1) {
        if(command_handle_set(st_cmd, argv+1, argc-1) == FAIL)
            text_change(st_cmd->st_text, st_cmd->error);
        else
            text_change(st_cmd->st_text, st_cmd->text);
    } else
    if(strcmp(argv[0], "quit") == 0) {
        sdl_stop(st_cmd->st_sdl_parent);
    } else
        text_change(st_cmd->st_text, st_cmd->text);

    for(i = 0; i < argc; i++)
        free(argv[i]);
    free(argv);


    return SUCCESS;
}

int command_handle_set(struct command_context* st_cmd, char** argv, int argc) {
    int w, h;

    if(strcmp(argv[0], "resolution") == 0) {
        if(argc == 3) {
            w = atoi(argv[1]);
            h = atoi(argv[2]);

            if(w > 0 && h > 0)
                sdl_resolution_set(st_cmd->st_sdl_parent, w, h);
            else {
                command_set_error(st_cmd, "Invalid resolution.");
                return FAIL;
            }
        } else {
            command_set_error(st_cmd, "Invalid number of arguments.");
            return FAIL;
        }
    }

    return SUCCESS;
}
