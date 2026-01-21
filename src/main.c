#include <stdlib.h>


#include "render/render.h"
#include "character/player.h"


float braking_power = 0.7f;

int main(void) {
    window_t *win = create_window(1080, 720, "Rouge souls");
    texture_t *helmet = create_texture("assets/helmet.png");

    player_t player = create_player(helmet, create_stats(1.0f));

    init_textures();

    rect_t *player_rect = get_player_rect(&player);

    while (is_window_open(win)) {
        clear_window(float3(0.0f, 0.0f, 0.0f));

        if (key_pressed(win, keycode_w)) player.velocity.y += player.stats.speed;
        if (key_pressed(win, keycode_s)) player.velocity.y -= player.stats.speed;
        if (key_pressed(win, keycode_d)) player.velocity.x += player.stats.speed;
        if (key_pressed(win, keycode_a)) player.velocity.x -= player.stats.speed;

        if (key_pressed(win, keycode_f)) toggle_fullscreen(win);
        
        player_rect->position.x += player.velocity.x * delta_time;
        player_rect->position.y += player.velocity.y * delta_time;

        player.velocity.x *= braking_power;
        player.velocity.y *= braking_power;

        draw_sprite(win, player.sprite);

        update_window(win);
    };

    close_window(win);
    free(win);
    free(helmet);
    return 0;
}
