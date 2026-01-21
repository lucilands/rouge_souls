#ifndef _PLAYER_H
#define _PLAYER_H
#include "stats.h"
#include "../render/render.h"

typedef struct player {
    stats_t stats;
    
    sprite_t sprite;
    float2_t velocity;
} player_t;

player_t create_player(texture_t *tex, stats_t stats);
rect_t *get_player_rect(player_t *p);

#endif //_PLAYER_H
