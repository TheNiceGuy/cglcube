#include "config.h"
#include "sdl.h"

int main(int argc, char* argv[]) {
    struct sdl_context st_sdl;

    sdl_init(&st_sdl);
    sdl_start(&st_sdl);
    sdl_handle_event(&st_sdl);
    sdl_stop(&st_sdl);

    return 0;
}
