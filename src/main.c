#include "config.h"
#include "sdl.h"
#include "render.h"

int main() {
    struct sdl_context st_sdl;

    sdl_display_version(&st_sdl);

    sdl_init(&st_sdl);
    sdl_start(&st_sdl);

    sdl_handle_event(&st_sdl);

    return 0;
}
