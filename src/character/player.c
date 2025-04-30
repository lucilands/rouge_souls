#include "player.h"



player_t create_player(texture_t *tex, stats_t stats) {
    return (player_t) {
        stats,
        create_sprite(0.4f, 0.4f, 0.0f, 0.0f, tex)
    };
}

rect_t *get_player_rect(player_t *p) {
    return &p->sprite.rect;
}
