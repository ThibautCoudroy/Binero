#include <gb/gb.h>
#include "texts.h"

#define W_TILES_READY 7
#define W_TILES_GO 4

const uint8_t loading_bar[] = {0x1C, 0x1D, 0x1D, 0x1D, 0x1D, 0x1D, 0x1E};

void setup_texts() {
    hide_text();
    for(uint8_t i=8, j=6; i<37; i++, j++) {
        set_sprite_tile(i, j);
        set_sprite_prop(i, 5);
    }
    for(uint8_t i=0, j=8 + 2 * W_TILES_READY + 2 * W_TILES_GO; i<7; i++, j++) {
        set_sprite_tile(j, loading_bar[i]);
    }
}

void show_text_ready() {
    for(uint8_t i=0; i<2 * W_TILES_READY; i++) {
        move_sprite(i + 8, 62 + 8 * (i % W_TILES_READY), (i >= W_TILES_READY) ? 96 : 88);
        if(i<7) move_sprite(i + 30, 60 + 8 * i, 104);
    }
}

void hide_text() {
    for(uint8_t i = 0; i < 8 + 2 * W_TILES_READY + 2 * W_TILES_GO + 7; i++) move_sprite(i, 0, 0);
}

void show_text_go() {
    for(uint8_t i = 0; i < 2 * W_TILES_GO; i++) {
        move_sprite(i + 2 * W_TILES_READY + 8, 74 + 8 * (i % W_TILES_GO), (i >= W_TILES_GO) ? 96 : 88);
    }
}
