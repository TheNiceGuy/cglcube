#include "sdl.h"
#include "render.h"
#include "draw.h"

int main() {
    struct sdl_context st_sdl;

    sdl_display_version(&st_sdl);

    sdl_init(&st_sdl);
    render_init(&st_sdl.st_render, 400, 400);

    sdl_start(&st_sdl);
    sdl_handle_event(&st_sdl);

    return 0;
}
