#ifndef COMMAND_H
#define COMMAND_H

#include <stdlib.h>
#include "text.h"

#define CMD_BUFFER 64
#define ERR_BUFFER 64

struct command_context {
    /*
     * Contains the current written command.
     */
    char* text;
    /*
     * Contains an error if an error happened.
     */
    char* error;
    /*
     * Contains the position of the cursor in the text buffer.
     */
    int cursor;
    /**
     * Points to the parent SDL context of the command context.
     */
    struct sdl_context* st_sdl_parent;
    /**
     * Points to the text structure that is linked with the command context.
     */
    struct text* st_text;
};

/**
 * This function initialises the command context.
 *
 * @param st_sdl A pointer to an command_context structure.
 */
void command_init(struct command_context* st_cmd);

/**
 * This function links the command context with an SDL context that should be
 * its parent.
 *
 * @param st_cmd A pointer to an command_context structure.
 * @param st_sdl A pointer to an sdl_context structure.
 */
void command_link_sdl(struct command_context* st_cmd, struct sdl_context* st_sdl);

/**
 * This function links the command context with an text structure.
 *
 * @param st_cmd A pointer to an command_context structure.
 * @param st_sdl A pointer to an text structure.
 */
void command_link_text(struct command_context* st_cmd, struct text* st_text);

/**
 * This function frees memory of the command context.
 *
 * @param st_cmd A pointer to an command_context structure.
 */
void command_free(struct command_context* st_cmd);

/**
 * This function clears the text in the command context.
 *
 * @param st_cmd A pointer to an command_context structure.
 */
void command_clear(struct command_context* st_cmd);

/**
 * This function cancels the input and clear the text.
 *
 * @param st_cmd A pointer to an command_context structure.
 */
void command_cancel(struct command_context* st_cmd);

/**
 * This function sets the error message in the structure.
 *
 * @param st_cmd A pointer to an command_context structure.
 * @param error A pointer to the error message.
 */
void command_set_error(struct command_context* st_cmd, char* error);

/**
 * This function appends text to the command's text.
 *
 * @param st_cmd A pointer to an command_context structure.
 * @param input A pointer to the text.
 * @return An error code defined in config.h.
 */
int command_append(struct command_context* st_cmd, char* input);

/**
 * This function removes one character on the command's text.
 *
 * @param st_cmd A pointer to an command_context structure.
 * @return An error code defined in config.h.
 */
int command_backspace(struct command_context* st_cmd);

/**
 * This function clears and executes the written command.
 *
 * @param st_cmd A pointer to an command_context structure.
 * @return An error code defined in config.h.
 */
int command_execute(struct command_context* st_cmd);

/**
 * This function handles 'set' command.
 *
 * @param st_cmd A pointer to an command_context structure.
 * @param argv A pointer to the table of arguments.
 * @param argc The number of arguments.
 * @return An error code defined in config.h.
 */
int command_handle_set(struct command_context* st_cmd, char** argv, int argc);

#endif
