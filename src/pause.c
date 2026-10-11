#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include "audio/audio.h"
#include "pause.h"
#include "timer.h"
#include "graphics/blank.h"
#include "graphics/font.h"

#define __ 27
#define SPACE 27
#define ARROW 38

const uint8_t pause_time[] = {T,  I,  M, E};
const uint8_t pause_game_paused[] = {
    __,  6,  0, 12,  4, __,
    15,  0, 20, 18,  4,  3,
};

const uint8_t pause_text[] = {
     C, O, N, T, I, N, U, E,
    __, __, __, __, __, __, __, __,
    N, E, W, __, G, A, M, E,
    __, __, __, __, __, __, __, __,
    M, E, N, U, __, __, __, __
};

uint8_t pause() {
    update_visual_timer(false);
    set_win_tile_xy(4, 9, ARROW);

    SHOW_WIN;
    HIDE_SPRITES;
    play_sound_pause_1();

    uint8_t pause_frames = 0;
    uint8_t sel = 0;
    bool padup = false;

    while(1) {
        if (!joypad() || padup) {
            padup = true;
            uint8_t buttons = joypad();
            bool cond1 = (buttons & J_UP) && sel>0;
            bool cond2 = (buttons & J_DOWN) && sel<2;
            if(cond1 | cond2) {
                pause_frames = 64;   // end sound because another is playing
                set_win_tile_xy(4, sel*2 + 9, SPACE);
                if(cond2) sel++;
                else sel--;
                set_win_tile_xy(4, sel*2 + 9, ARROW);
                play_sound_sel();
                waitpadup();
            }
            if(buttons & (J_SELECT | J_START | J_A)) {    // Select opt
                waitpadup();
                break;
            }
        }
        
        wait_vbl_done();
        if(pause_frames == 3) play_sound_pause_2();
        if(pause_frames < 64) pause_frames++;
    }

    play_sound_sel_valid();
    if(!sel) {          // if continue
        HIDE_WIN;
        SHOW_SPRITES;
    }

    return (sel << 1);  // 00x = continue, 01x = reset, 10x = back to menu
}