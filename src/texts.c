#include <gb/gb.h>
#include "texts.h"
#include "graphics/font.h"

#define W_TILES_READY 7
#define W_TILES_GO 4

const uint8_t loading_bar[] = {0x1C, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1E};

void setup_texts(void) {
    hide_text();
    for(uint8_t i=8, j=6; i<40; i++, j++) {
        set_sprite_tile(i, j);
        set_sprite_prop(i, 5);
    }
    for(uint8_t i=0, j=8 + 2 * W_TILES_READY + 2 * W_TILES_GO; i<10; i++, j++) {
        set_sprite_tile(j, loading_bar[i]);
    }
}

void show_text_ready(void) {
    for(uint8_t i=0; i<2 * W_TILES_READY; i++) {
        // "Ready"
        move_sprite(i + 8, 62 + 8 * (i % W_TILES_READY), (i >= W_TILES_READY) ? 96 : 88);
        // Loading bar
        if(i<10) move_sprite(i + 30, 61 + 5 * i, 104);
    }
}

void hide_text(void) {
    for(uint8_t i = 0; i < 40; i++) hide_sprite(i);
}

void show_text_go(void) {
    for(uint8_t i = 0; i < 2 * W_TILES_GO; i++) {
        move_sprite(i + 2 * W_TILES_READY + 8, 74 + 8 * (i % W_TILES_GO), (i >= W_TILES_GO) ? 96 : 88);
    }
}

#define __ 27

const uint8_t err_msg[] =
{
    T, H, I, S,__, G, A, M, E,__, I, S,__, O, N, L, Y,
    D, E, S, I, G, N, E, D, __,F, O, R,__, T, H, E,__,
    __,G, A, M, E,__, B, O, Y,__, C, O, L, O, R,__,__,
};

const uint8_t rules_header_text[] = {R, U, L, E, S};

const uint8_t rules_text[] =
{
    __,__, A, P, P, L, Y,__, F, O, R,__, R, O, W, S,
    __,__,__, A, N, D,__, C, O, L, U, M, N, S,__,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
    42,__, D, O,__, N, O, T,__, A, L, I, G, N,__,31,
    __,__, B, L, U, E,__, O, R,__,31,__, P, I, N, K,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
    43,__,__,31,__, B, L, U, E,__, A, N, D,__,31,__,
    __,__, P, I, N, K,__, P, E, R,__, L, I, N, E,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
    44,__,__,__, E, A, C, H,__, L, I, N, E,__,__,__,
    __,__,__,__, I, S,__, U, N, I, Q, U, E,__,__,__,
};

const uint8_t controls_header_text[] = {C, O, N, T, R, O, L, S};

const uint8_t controls_text[] =
{
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
     A,__,26,__, C, H, A, N, G, E,__, T, I, L, E,39, S,
    __,__,__,__, C, O, L, O, R,__,__,__,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
     B,__,26,__, U, N, D, O,__, C, H, A, N, G, E,__,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
     A, R, R, O, W, S,__,26,__, M, O, V, E,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
     S, T, A, R, T,__,__,__,__,__,__,__,__,__,__,__,__,
    __,__, O, R,__,__,__,26,__, P, A, U, S, E,__,__,__,
     S, E, L, E, C, T,__,__,__,__,__,__,__,__,__,__,__,
};

const uint8_t password_text[] =
{
    S, T, A, R, T,
    __, __, __, __, __,
    E, D, I, T, __,
    __, __, __, __, __,
    M, E, N, U, __,
};
