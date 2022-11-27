#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/gbdecompress.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <rand.h>
#include "game.h"
#include "audio/audio.h"
#include "graphics/blank.h"
#include "graphics/font.h"
#include "graphics/intro.h"
#include "graphics/menu.h"
#include "graphics/options.h"
#include "graphics/takuzu_tiles.h"
#include "graphics/window_outline.h"
#include "data/puzzles_data.h"

#define LOGO_OFFSET 51

uint16_t seed = 0;
uint16_t code = 0;
bool fixed_seed = false;

uint8_t das = 18;

uint8_t high_score_names[3][8] =
{
    { 5, 17,  0, 13, 10, 26, 26, 26},
    {15,  4, 19,  4, 17, 26, 26, 26},
    {12,  0, 17,  8,  0, 13, 13,  4}
};

// Times in frames (60 frames = 1 sec)
uint16_t high_score_times[] = {3600, 7200, 18000};

// Text input variables
uint8_t new_time_pos = 3;
uint8_t char_id = 0;
uint8_t flicker_counter = 0;
uint8_t char_count, char_range;
uint8_t nb[4] = {0, 0, 0, 0};

uint8_t password_text[] =
{
    0x12, 0x13, 0x00, 0x11, 0x13,
    0x1B, 0x1B, 0x1B, 0x1B, 0x1B,
    0x04, 0x03, 0x08, 0x13, 0x1B,
    0x1B, 0x1B, 0x1B, 0x1B, 0x1B,
    0x0C, 0x04, 0x0D, 0x14, 0x1B,
};

uint8_t err_msg[] =
{
    T, H, I, S,27, G, A, M, E,27, I, S,27, O, N, L, Y,
    D, E, S, I, G, N, E, D, 27,F, O, R,27, T, H, E,27,
    27,G, A, M, E,27, B, O, Y,27, C, O, L, O, R,27,27,
};

uint8_t rules_header_text[] = {R, U, L, E, S};

uint8_t rules_text[] =
{
    __,__, A, P, P, L, Y,__, F, O, R,__, R, O, W, S,
    __,__,__, A, N, D,__, C, O, L, U, M, N, S,__,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
    42,__, E, A, C, H,__, L, I, N, E,__, I, S,__,__,
    __,__, D, I, F, F, E, R, E, N, T,__,__,__,__,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
    43,__,31,__, B, L, U, E,__, A, N, D,__,31,__,__,
    __,__, P, I, N, K,__, P, E, R,__, L, I, N, E,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,__,__,
    44,__, D, O,__, N, O, T,__, A, L, I, G, N,__,31,
    __,__, B, L, U, E,__, O, R,__,31,__, P, I, N, K,
};

uint8_t controls_header_text[] = {C, O, N, T, R, O, L, S};

uint8_t controls_text[] =
{
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

void intro_screen() {
    SHOW_BKG;
    HIDE_WIN;
    HIDE_SPRITES;

    // Load "TiTi" tiles
    set_bkg_1bpp_data(LOGO_OFFSET, 7, intro_1bpp_tiles);
    // Load "Binero" tiles
    gb_decompress_bkg_data(LOGO_OFFSET + 7, intro_binero_tiles_compressed);

    DISPLAY_ON;
    VBK_REG = 0;

    for(uint8_t i=0; i<18; i++) {
        set_bkg_based_tiles(0, i, 20, 1, blank_6, LOGO_OFFSET);
    }
    set_bkg_based_tiles(4, 6, 12, 5, intro_logo, LOGO_OFFSET + 6);
    set_bkg_based_tiles(7, 1, 6, 3, intro_titi, LOGO_OFFSET);
    set_bkg_tiles(4, 13, 13, 4, intro_text);

    VBK_REG = 1;
    for(uint8_t i=6; i<11; i++) {
        set_bkg_tiles(0, i, 20, 1, blank_1);
    }
    
    // Set palettes of "Binero"
    set_bkg_tile_xy(5, 7, 2);
    set_bkg_tile_xy(5, 9, 3);
    set_bkg_tile_xy(14, 9, 2);
    set_bkg_tile_xy(7, 7, 4);

    // Flipping tiles in "TITI" logo
    set_bkg_tile_xy(7, 3, 0x40);                    // Bottom left
    set_bkg_tiles(12, 1, 1, 3, intro_titi_flips);   // Right part

    VBK_REG = 0;
    for(uint8_t i=0; i<5; i++) set_bkg_palette(i, 1, intro_palettes);   // Gray palettes

    // Part 1 - Main title / Part 2 - Logo & Text
    for(uint8_t j=1; j!=255; j--) {
        delay(1000);    // Wait between parts (and at beginning)
        for(uint8_t i=1; i<intro_palettes_nb; i++) {
            set_bkg_palette(j, j ? 4 : 1, &intro_palettes[(i << 4) - 12]);    // Replace by the palette of next step
            if(i < intro_palettes_nb-1) delay(100);
        }
    }
}

void show_char() {
    set_bkg_tile_xy(3 + char_id, 3 + 2 * new_time_pos, high_score_names[new_time_pos][char_id]);
}

void update_char_flicker() {
    if(flicker_counter >= 20) {
        set_bkg_tile_xy(3 + char_id, 3 + 2 * new_time_pos, 0x1B);
    } else {
        show_char();
    }
}

void show_pass_char() {
    // Offset to numbers or letters depending on hex value
    uint8_t temp = nb[char_id] < 10 ? (nb[char_id] + 28) : (nb[char_id] - 10);
    set_win_tile_xy(8 + char_id, 6, temp);
}

void update_pass_char_flicker() {
    if(flicker_counter >= 20) {
        set_win_tile_xy(8 + char_id, 6, 0x1B);
    } else {
        show_pass_char();
    }
}

void text_input(uint8_t window) {
    void (*show_text_char)();
    if(window == 0) {
        char_count = 8;
        char_range = 27;
        show_text_char = show_char;
    } else {
        char_count = 4;
        char_range = 15;
        show_text_char = show_pass_char;
    }
    uint8_t buttons = 0, old_buttons, delta;
    uint8_t hold_frames = 0;
    char_id = flicker_counter = 0;
    bool hold_up = false;
    while(1) {
        old_buttons = buttons;
        buttons = joypad();
        delta = (~old_buttons) & buttons;
        // Move cursor
        if(delta & (J_A | J_RIGHT)) {
            show_text_char();
            play_sound_char();
            char_id = (char_id + 1) % char_count;
        } else if(delta & (J_B | J_LEFT)) {
            show_text_char();
            play_sound_char();
            char_id = (char_id + char_count - 1) % char_count;
        }
        // Update char
        uint8_t temp;
        if(window == 0) {
            temp = high_score_names[new_time_pos][char_id];
        } else {
            temp = nb[char_id];
        }

        if(delta & J_UP) {
            temp++;
            hold_up = true;
            hold_frames = 1;
            play_sound_char();
        } else if(delta & J_DOWN) {
            temp += char_range;
            hold_up = false;
            hold_frames = 1;
            play_sound_char();
        } else if(hold_up && (buttons & J_UP)) {
            hold_frames++;
            if(hold_frames == 10) {
                hold_frames = 0;
                temp++;
                play_sound_char();
            }
        } else if(!hold_up && (buttons & J_DOWN)) {
            hold_frames++;
            if(hold_frames == 10) {
                hold_frames = 0;
                temp += char_range;
                play_sound_char();
            }
        } else {
            hold_frames = 0;
        }
        if(window == 0) {
            high_score_names[new_time_pos][char_id] = temp % 28;
            update_char_flicker();
        } else {
            nb[char_id] = temp % 16;
            update_pass_char_flicker();
        }
        
        if(buttons & (J_START | J_SELECT)) {
            show_text_char();
            play_sound_sel_valid();
            break;
        }
        wait_vbl_done();
        flicker_counter = (flicker_counter + 1) % 30;
    }
}

void add_name() {
    // Temp name is "-" only
    for(uint8_t i=0; i<8; i++) high_score_names[new_time_pos][i] = 26;
    // Visual update
    set_bkg_tiles(3, 3 + 2*new_time_pos, 8, 1, &high_score_names[new_time_pos][0]);
    text_input(0);
}

void set_score_time(uint8_t y, uint16_t time) {
    uint8_t min = time / 3600;
    uint8_t sec = (time / 60) % 60;
    uint8_t ticks = ((uint16_t) (time % 60) * 5) / 3;
    int8_t high_score_display[8] = {min/10, min%10, 11, sec/10, sec%10, 12, ticks/10 + 13, ticks%10 + 13};
    set_bkg_based_tiles(12, y, 5, 1, high_score_display + (min ? 0 : 3), 28);
}

void update_scores() {
    for(uint8_t i=0; i<3; i++) {
        if(i != new_time_pos) {
            set_bkg_tiles(3, 3 + 2*i, 8, 1, &high_score_names[i][0]);
        }
        set_score_time(3 + 2*i, high_score_times[i]);
    }
    if(new_time_pos < 3) add_name();
}

uint8_t add_score(uint16_t time) {
    for(uint8_t i=0; i<3; i++) {
        if(time < high_score_times[i]) {
            for(uint8_t j = 2; j > i; j--) {
                for(uint8_t k=0; k<8; k++) {
                    high_score_names[j][k] = high_score_names[j-1][k];
                }
                high_score_times[j] = high_score_times[j-1];
            }
            high_score_times[i] = time;
            return i;
        }
    }
    return 3;
}

void menu_setup() {
    DISPLAY_OFF;
    HIDE_WIN;
    set_bkg_palette(0, 3, menu_palette);
    // Loading game tiles and font with other colors only once
    set_1bpp_colors(2, 0);
    set_bkg_1bpp_data(41, 10, font + 224);
    gb_decompress_bkg_data(LOGO_OFFSET, takuzu_tiles_compressed);   // Takuzu tiles
    VBK_REG = 0;

    for(uint8_t i=0; i<18; i++) set_bkg_tiles(0, i, 20, 1, blank_27);
    draw_window(2, 2, 16, 7, true);
    set_bkg_tiles(5, 1, 10, 1, menu_highscore);
    set_bkg_tiles(5, 10, 8, 7, menu_text);
    
    VBK_REG = 1;
    for(uint8_t i=0; i<18; i++) set_bkg_tiles(0, i, 20, 1, blank_0);
    VBK_REG = 0;
    DISPLAY_ON;
    update_scores();
    waitpadup();
}

void setup_menu_win(){
    VBK_REG = 1;
    for(uint8_t i=0; i<18; i++) set_win_tiles(0, i, 20, 1, blank_0);
    VBK_REG = 0;
    for(uint8_t i=0; i<18; i++) set_win_tiles(0, i, 20, 1, blank_27);
}

uint8_t password_menu() {
    // Load password window
    setup_menu_win();
    draw_window(4, 2, 12, 7, false);
    set_win_tiles(8, 11, 5, 5, password_text);
    set_win_tiles(6, 4, 8, 1, menu_text + 16);
    for(char_id = 0; char_id < 4; char_id++) show_pass_char();

    SHOW_WIN;
    while(1) {
        set_win_tile_xy(6, 13, 27);
        text_input(1);
        waitpadup();
        // Player can go back to menu
        uint8_t sel = 0;
        set_win_tile_xy(6, 11 + 2*sel, 38);
        while(1) {
            uint8_t buttons = joypad();
            bool cond1 = (buttons & J_UP) && sel>0;
            bool cond2 = (buttons & J_DOWN) && sel<2;
            if(cond1 | cond2) {
                set_win_tile_xy(6, 11 + 2*sel, 27);
                if(cond2) sel++;
                else sel--;
                set_win_tile_xy(6, 11 + 2*sel, 38);
                play_sound_sel();
                waitpadup();
            }

            if(buttons & (J_SELECT | J_START | J_A)) {
                // If sel == 1, back to input loop
                waitpadup();
                play_sound_sel_valid();
                if(sel == 1) break;
                if(sel == 0) {
                    code = (nb[0] << 12) | (nb[1] << 8) | (nb[2] << 4) | nb[3];
                    DISPLAY_OFF;
                }
                HIDE_WIN;
                return sel; // Returns 2 if menu, 0 if play
            }
        }
    }
}

void update_visual_on_off() {
    set_win_tiles(13, 11, 3, 1, sound_value() ? options_on : options_off);
    set_win_tiles(13, 13, 3, 1, music_value() ? options_on : options_off);
}

void update_visual_das() {
    set_win_tile_xy(14, 9, das / 6 + 0x1C);
}

void show_das_text() {
    set_win_tiles(1, 3, 18, 1, options_text_das);
    set_win_tiles(3, 5, 13, 2, options_text_das2);
}

void options_menu() {
    setup_menu_win();
    set_win_tiles(6, 1, 7, 1, menu_text + 32);
    set_win_tiles(6, 9, 5, 7, options_text);

    VBK_REG = 1;
    for(uint8_t i=3; i<7; i++) set_win_tiles(0, i, 20, 1, blank_1);
    VBK_REG = 0;
    show_das_text();

    update_visual_on_off();
    update_visual_das();
    draw_window(3, 8, 14, 9, false);
    set_win_tile_xy(4, 9, 38);

    SHOW_WIN;

    uint8_t sel = 0;
    while(1) {
        uint8_t buttons = joypad();
        bool cond1 = (buttons & J_UP) && sel>0;
        bool cond2 = (buttons & J_DOWN) && sel<3;
        if(cond1 | cond2) {
            set_win_tile_xy(4, 9 + 2*sel, 27);
            if(cond2) sel++;
            else sel--;
            set_win_tile_xy(4, 9 + 2*sel, 38);
            if(sel == 0) {
                show_das_text();
            } else {
                for(uint8_t i=3; i<=6; i++) set_win_tiles(0, i, 20, 1, blank_27);
            }
            play_sound_sel();
            waitpadup();
        }
        if(sel == 0) {
            if(buttons & J_LEFT) {
                das = (das + 54) % 60;
                update_visual_das();
                play_sound_sel_valid();
                waitpadup();
            } else if(buttons & (J_RIGHT | J_SELECT | J_START | J_A)) {
                das = (das + 6) % 60;
                update_visual_das();
                play_sound_sel_valid();
                waitpadup();
            }
        } else if(sel == 3) {
            if(buttons & (J_SELECT | J_START | J_A)) {
                waitpadup();
                play_sound_sel_valid();
                if(sel == 3) break;
            }
        } else {
            if(buttons & (J_SELECT | J_START | J_A | J_LEFT | J_RIGHT)) {
                if(sel == 1) switch_sound();
                else if(sel == 2) switch_music();

                update_visual_on_off();
                play_sound_sel_valid();
                waitpadup();
            }
        }
    }

    HIDE_WIN;
}

void rules_controls_menu() {
    // Rules
    setup_menu_win();
    draw_window(7, 1, 7, 3, false);
    set_win_tiles(8, 2, 5, 1, rules_header_text);
    set_win_tiles(1, 5, 16, 11, rules_text);
    SHOW_WIN;
    waitpad(0xFF);
    waitpadup();
    play_sound_sel_valid();

    // Controls
    setup_menu_win();
    draw_window(5, 1, 10, 3, false);
    set_win_tiles(6, 2, 8, 1, controls_header_text);
    set_win_tiles(1, 6, 17, 10, controls_text);
    waitpad(0xFF);
    waitpadup();
    play_sound_sel_valid();
    HIDE_WIN;
}

void menu() {
    while(1) {
        menu_setup();
        uint8_t sel = 0;
        set_bkg_tile_xy(3, 10, 38);
        while(1) {
            uint8_t buttons = joypad();
            bool cond1 = (buttons & J_UP) && sel>0;
            bool cond2 = (buttons & J_DOWN) && sel<3;
            if(cond1 | cond2) {
                set_bkg_tile_xy(3, 10 + 2*sel, 27);
                if(cond2) sel++;
                else sel--;
                set_bkg_tile_xy(3, 10 + 2*sel, 38);
                play_sound_sel();
                waitpadup();
            }
            if(buttons & (J_SELECT | J_START | J_A)) {
                waitpadup();
                play_sound_sel_valid();
                // Play
                if(sel == 0) {
                    fixed_seed = false;
                    break;
                }
                // Password (breaks if not aborted)
                if(sel == 1) {
                    uint8_t back_menu = password_menu();
                    if(back_menu != 2) {
                        fixed_seed = true;
                        break;
                    }
                }
                // Opt
                if(sel == 2) {
                    options_menu();
                }
                // Rules
                if(sel == 3) {
                    rules_controls_menu();
                }
            }
            waitpad(0xFF);
        }

        // Setup new game
        set_bkg_tile_xy(3, 11 + 2*sel, 27);
        if(fixed_seed) {
            initrand(code);
        } else {
            seed = (seed << 8) + DIV_REG;
            initrand(seed);
        }
        // Game start
        uint16_t new_time = game_screen(das, fixed_seed);
        // Game ended. "0" indicates game hasn't ended.

        // Adds new time to leaderboard & keeps position of new score
        new_time_pos = new_time ? add_score(new_time) : 3;

    }
}

// Gets stuck in waiting screen if not CGB
void check_if_cgb(void) {
    if (_cpu != CGB_TYPE) {
        for(uint8_t i=0; i<18; i++) set_bkg_tiles(0, i, 20, 1, blank_27);
        set_bkg_tiles(2, 7, 17, 3, err_msg);
        DISPLAY_ON;
        SHOW_BKG;
        while(1) {
            wait_vbl_done();
        }
    }
}

void main() {
    DISPLAY_OFF;
    // Loading font
    set_1bpp_colors(3, 0);
    set_bkg_1bpp_data(0, 41, font);
    check_if_cgb();

    intro_screen();
    waitpadup();    // If player already pressed something, wait release for random seed
    waitpad(0xFF);
    seed = DIV_REG;
    waitpadup();
    sound_on();
    menu();
}
