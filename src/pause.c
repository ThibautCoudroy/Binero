#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include "audio/audio.h"
#include "pause.h"
#include "timer.h"
#include "graphics/blank.h"

#define __ 27
#define SPACE 27
#define ARROW 38

uint8_t pause_time[] = {19,  8,  12, 4};
uint8_t pause_game_paused[] = {
    __,  6,  0, 12,  4, __,
    15,  0, 20, 18,  4,  3,
};

uint8_t pause_text[] = {
     2, 14, 13, 19,  8, 13, 20,  4,
    __, __, __, __, __, __, __, __,
    13,  4, 22, __,  6,  0, 12,  4,
    __, __, __, __, __, __, __, __,
    12, 04, 13, 20, __, __, __, __
};

uint8_t pause_frames;

uint8_t pause() {
    update_visual_timer(false);
    set_win_tile_xy(4, 9, ARROW);

    waitpadup();
    SHOW_WIN;
    HIDE_SPRITES;

    pause_frames = 0;

    uint8_t sel = 0;
    while(1) {
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
            break;
        }
        wait_vbl_done();
        
        if(pause_frames == 0) play_sound_pause_1();
        else if(pause_frames == 3) play_sound_pause_2();
        if(pause_frames < 64) pause_frames++;
    }
    waitpadup();
    play_sound_sel_valid();
    if(!sel) {          // if not continue
        HIDE_WIN;
        SHOW_SPRITES;
    }

    return (sel << 1);  // 00x = continue, 01x = reset, 10x = back to menu
}