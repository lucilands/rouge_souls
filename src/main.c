#include <stdio.h>
#include <stdlib.h>


#include "render/render.h"
#include "character/player.h"


int main(void) {
    window_t *win = create_window(1080, 720, "Rouge souls");
    texture_t *helmet = create_texture("assets/helmet.png");

    player_t player = create_player(helmet, create_stats(1.0f));

    init_textures();
    while (is_window_open(win)) {
        clear_window(float3(1.0f, 1.0f, 1.0f));

        if (key_pressed(win, keycode_w)) get_player_rect(&player)->position.y += 1.0f * delta_time;
        if (key_pressed(win, keycode_s)) get_player_rect(&player)->position.y -= 1.0f * delta_time;
        if (key_pressed(win, keycode_d)) get_player_rect(&player)->position.x += 1.0f * delta_time;
        if (key_pressed(win, keycode_a)) get_player_rect(&player)->position.x -= 1.0f * delta_time;

        if (key_pressed(win, keycode_f)) toggle_fullscreen(win);

        draw_sprite(win, player.sprite);

        update_window(win);
    };

    close_window(win);
    free(win);
    free(helmet);
    return 0;
}
