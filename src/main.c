#include <stdio.h>
#include <stdlib.h>


#include "render/render.h"


int main(void) {
    window_t *win = create_window(1080, 720, "Rouge souls");
    texture_t *helmet = create_texture("assets/helmet.png");

    sprite_t player = create_sprite(0.4f, 0.4f, 0, 0, helmet);

    init_textures();
    while (is_window_open(win)) {
        clear_window(float3(1.0f, 1.0f, 1.0f));

        if (key_pressed(win, keycode_w)) player.rect.position.y += 1.0f * delta_time;
        if (key_pressed(win, keycode_s)) player.rect.position.y -= 1.0f * delta_time;
        if (key_pressed(win, keycode_d)) player.rect.position.x += 1.0f * delta_time;
        if (key_pressed(win, keycode_a)) player.rect.position.x -= 1.0f * delta_time;

        if (key_pressed(win, keycode_f)) toggle_fullscreen(win);

        draw_sprite(win, player);

        update_window(win);
    };

    close_window(win);
    free(win);
    free(helmet);
    return 0;
}
