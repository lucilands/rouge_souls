#ifndef _RENDER_H
#define _RENDER_H
#include <stdbool.h>

#include "../utils/math.h"
#include "../keycodes.h"

typedef struct window window_t;
typedef struct render_params render_params_t;
typedef struct texture texture_t;

typedef struct rect {
    float2_t size;
    float2_t position;
    float rotation;
    float2_t origin;
    float3_t color;
    render_params_t *params;
} rect_t;

typedef struct sprite {
    rect_t rect;
    texture_t *tex;
} sprite_t;

extern uint default_texture;
extern double delta_time;

window_t *create_window(unsigned int width, unsigned int height, const char *title);
bool is_window_open(window_t *win);

int2_t get_window_size(window_t *win);

void update_window(window_t *win);
void close_window(window_t *win); 
void clear_window(float3_t color);
void toggle_fullscreen(window_t *win);

void init_textures();

texture_t *create_texture(const char *path);
texture_t *create_texture_d(const unsigned char *data, int w, int h, int n);
void use_texture(render_params_t *params, texture_t *tex);

uint get_render_params_shader(render_params_t *params);

rect_t create_rect(float w, float h, float x, float y);
void draw_rect(window_t *win, rect_t r);

sprite_t create_sprite(float w, float h, float x, float y, texture_t *tex);
void draw_sprite(window_t *win, sprite_t s);

double get_time();

bool key_pressed(window_t *win, key key_code);

#endif //_RENDER_H
