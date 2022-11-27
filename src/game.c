#include <gb/gb.h>
#include <gb/cgb.h>
#include <gb/gbdecompress.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <rand.h>
#include "game.h"
#include "graphics/window_outline.h"
#include "graphics/takuzu_tiles.h"
#include "graphics/takuzu_bg.h"
#include "graphics/takuzu_sprites.h"
#include "graphics/font.h"
#include "graphics/blank.h"
#include "texts.h"
#include "timer.h"
#include "pause.h"
#include "audio/audio.h"
#include "puzzles_gen.h"

#define NB_MOVES_HIST 30
#define TILES_OFFSET 51
#define FLICKER_ENTRY_NB 8

// Global variables (better to take less space)

const uint16_t bg_palette[] = {
    takuzu_tilesCGBPal0c0, takuzu_tilesCGBPal0c1,
    takuzu_tilesCGBPal0c2, takuzu_tilesCGBPal0c3,

    takuzu_tilesCGBPal1c0, takuzu_tilesCGBPal1c1,
    takuzu_tilesCGBPal1c2, takuzu_tilesCGBPal1c3,

    takuzu_tilesCGBPal2c0, takuzu_tilesCGBPal2c1,
    takuzu_tilesCGBPal2c2, takuzu_tilesCGBPal2c3,

    takuzu_tilesCGBPal3c0, takuzu_tilesCGBPal3c1,
    takuzu_tilesCGBPal3c2, takuzu_tilesCGBPal3c3,

    takuzu_tilesCGBPal4c0, takuzu_tilesCGBPal4c1,
    takuzu_tilesCGBPal4c2, takuzu_tilesCGBPal4c3,

    takuzu_tilesCGBPal5c0, takuzu_tilesCGBPal5c1,
    takuzu_tilesCGBPal5c2, takuzu_tilesCGBPal5c3,

    takuzu_tilesCGBPal6c0, takuzu_tilesCGBPal6c1,
    takuzu_tilesCGBPal6c2, takuzu_tilesCGBPal6c3,

    takuzu_tilesCGBPal7c0, takuzu_tilesCGBPal7c1,
    takuzu_tilesCGBPal7c2, takuzu_tilesCGBPal7c3,
};

const uint16_t sprites_palette[] = {
    sprites_tilesCGBPal0c0, sprites_tilesCGBPal0c1,
    sprites_tilesCGBPal0c2, sprites_tilesCGBPal0c3,

    sprites_tilesCGBPal1c0, sprites_tilesCGBPal1c1,
    sprites_tilesCGBPal1c2, sprites_tilesCGBPal1c3,

    sprites_tilesCGBPal2c0, sprites_tilesCGBPal2c1,
    sprites_tilesCGBPal2c2, sprites_tilesCGBPal2c3,

    sprites_tilesCGBPal3c0, sprites_tilesCGBPal3c1,
    sprites_tilesCGBPal3c2, sprites_tilesCGBPal3c3,

    sprites_tilesCGBPal4c0, sprites_tilesCGBPal4c1,
    sprites_tilesCGBPal4c2, sprites_tilesCGBPal4c3,

    0, 11627, 16912, 31710,

    0, 11627, 16912, 16165
};

uint8_t board_init[6][6];

uint8_t board[6][6];
/*board:
    dimensions : [i][j]
    bits :
    - .....x.. : init state (1 = true, 0 = false)
    - ......xx : pairs indicating color of each cell (00 gray, 01 blue, 10 pink)
*/
uint8_t board_flags[2][6];
/*board flags:
    dimensions : [col or line][flag color]
    bits :
    - xxxxxx.. : unused
    - ......xx : indicating flag (00 = unfinished, 01 = OK, 10 = KO, 11 = Duplicate)
*/

uint8_t cursor_x, cursor_y, cursor_frame;
uint8_t frames_held, dir, old_dir, mvt;
uint8_t ab, old_ab, delta_ab;
uint8_t ko_move_cntdwn, ok_move_cntdwn;
uint8_t game_state;
/*game_state bits:
    - .....x.. : back to menu boolean
    - ......x. : reset boolean
    - .......x : win boolean
*/
bool ok_move;
uint8_t *tile;

int8_t last_move_id;
uint8_t last_moves[NB_MOVES_HIST];
/*last_moves bits:
    - xxx..... : x pos
    - ...xxx.. : y pos
    - ......xx : tile color
*/

uint8_t das_value;
bool first_gen;

uint8_t win_anim_loops = 0;
uint8_t win_anim_frames = 0;


uint16_t flicker_palettes_values[][3] =
{
    {(7 << 8) + 1, 0x5ef7, 1057*26},
    {(7 << 8) + 2, 0x4631, 1057*26},
    {(7 << 8) + 3, 1057*14, 1057*18},
    {(0 << 8) + 3, 0x0000, 0x739C},
    {(1 << 8) + 3, 0x0000, 0x4210},
    {(2 << 8) + 3, 0x0000, 0x4210},
    {(3 << 8) + 3, 0x0000, 0x4210},
    {(4 << 8) + 3, 0x0000, 0x4210},
};
/* flicker_palettes_values words
    {x, y, z}
    x : palette entry (first byte) & col id (second byte)
    y : normal col
    z : alternate col
*/

void update_flags();
void update_visual_board();

void setup_board(bool fixed_seed) {
    // First time generating with fixed seed
    if(fixed_seed && first_gen) {
        // Generating in buffer
        generate_board(board_init);
    }
    
    // If fixed seed, don't regenerate board but copy board_init
    if(fixed_seed) {
        for(uint8_t i=0; i<36; i++) *(board[0] + i) = *(board_init[0] + i);
    } else {
        // Setup next board using random value
        generate_board(board);
    }
    
    // Reset flags (Checks if line or column already completed)
    for(uint8_t k=0; k<6; k++) {
        cursor_x = k;
        cursor_y = k;
        update_flags();
    }
    cursor_x = 2;
    cursor_y = 2;
    update_visual_board();
}

void setup_visual_board() {
    uint8_t col;
    VBK_REG = 1;
    for(uint8_t i=0; i<6; i++) {
        for(uint8_t j=0; j<6; j++) {
            col = board[j][i] & 3;
            set_bkg_tile_xy(4 + 2*i, 4 + 2*j, col);
            set_bkg_tile_xy(5 + 2*i, 4 + 2*j, col);
            set_bkg_tile_xy(4 + 2*i, 5 + 2*j, col);
            set_bkg_tile_xy(5 + 2*i, 5 + 2*j, col);
        }
    }
    VBK_REG = 0;
}

void get_flags_color(uint8_t j, uint8_t tiles_values[12]) {
    for(uint8_t i=0; i<6; i++) {
        uint8_t temp = board_flags[j][i] + 3;
        tiles_values[2*i] = temp;
        tiles_values[2*i + 1] = temp;
    }
}

void update_visual_board() {
    VBK_REG = 1;

    // UPDATE TILE
    uint8_t col = board[cursor_y][cursor_x] & 3;
    uint8_t temp[4] = {col, col, col, col};
    set_bkg_tiles(2*cursor_x + 4, 2*cursor_y + 4, 2, 2, temp);

    // UPDATE FLAGS
    uint8_t tiles_values[12];
    
    get_flags_color(0, tiles_values);
    set_bkg_tiles(4, 3, 12, 1, tiles_values);
    set_bkg_tiles(4, 16, 12, 1, tiles_values);
    
    get_flags_color(1, tiles_values);
    set_bkg_tiles(3, 4, 1, 12, tiles_values);
    set_bkg_tiles(16, 4, 1, 12, tiles_values);
    
    VBK_REG = 0;
}

void update_visual_cursor() {
    // Reset KO anim
    if(ko_move_cntdwn != 0) {
        for(uint8_t i=0; i<4; i++) {
            set_sprite_prop(i, get_sprite_prop(i) & 0xFE);
        }
        ko_move_cntdwn = 0;
    }
    uint8_t temp_x = (((cursor_x << 1) + 5) << 3) - 1;
    uint8_t temp_y = (((cursor_y << 1) + 6) << 3) - 2;
    move_sprite(0, temp_x, temp_y);
    move_sprite(2, temp_x, temp_y + 11);
    move_sprite(1, temp_x + 10, temp_y);
    move_sprite(3, temp_x + 10, temp_y + 11);
}

void update_visual_tile() {
    if(ok_move_cntdwn==0) {
        for(uint8_t i=4; i<=7; i++) move_sprite(i, 0, 0);
        return;
    }
    if(ok_move_cntdwn==20) {
        play_sound_move_ok();
        uint8_t x_offset = 40 + (cursor_x << 4);
        uint8_t y_offset = 48 + (cursor_y << 4);
        move_sprite(4, x_offset, y_offset);
        move_sprite(5, x_offset + 8, y_offset);
        move_sprite(6, x_offset, y_offset + 8);
        move_sprite(7, x_offset + 8, y_offset + 8);
        uint8_t col = (board[cursor_y][cursor_x] & 3) + 2;
        set_sprite_prop(4, col);
        set_sprite_prop(6, col);
        col += 32;
        set_sprite_prop(5, col);
        set_sprite_prop(7, col);
    }
    ok_move_cntdwn--;
}

void visual_tile_setup() {
    set_sprite_tile(4, 4);
    set_sprite_tile(5, 4);
    set_sprite_tile(6, 5);
    set_sprite_tile(7, 5);
}

void ko_move_cursor_anim() {
    if(ko_move_cntdwn == 0) return;
    uint8_t i;
    if(ko_move_cntdwn == 1) {
        // Back to palette 0
        for(i=0; i<4; i++) {
            set_sprite_prop(i, get_sprite_prop(i) & 0xFE);
            scroll_sprite(i, 1, 0);
        }
    }
    if(ko_move_cntdwn == 32) {
        play_sound_move_ko();
        // Set to palette 1
        for(i=0; i<4; i++) {
            set_sprite_prop(i, get_sprite_prop(i) ^ 1);
            scroll_sprite(i, -1, 0);
        }
    }
    if(ko_move_cntdwn & 1) {    // Every two frames
        int8_t dir = (ko_move_cntdwn & 0x04) ? 1 : -1;
        for(i=0; i<4; i++) scroll_sprite(i, dir, 0);
    }

    ko_move_cntdwn--;
}

void cursor_setup() {
    set_sprite_tile(0, 0);
    set_sprite_tile(1, 0);
    set_sprite_prop(0, 0b00000000);
    set_sprite_prop(2, 0b00000000);
    set_sprite_tile(2, 1);
    set_sprite_tile(3, 1);
    set_sprite_prop(1, 0b00100000);
    set_sprite_prop(3, 0b00100000);
}

void game_setup(bool fixed_seed) {
    DISPLAY_OFF;

    // Setting up variables
    old_dir = frames_held = dir = mvt = 0;
    ab = old_ab = delta_ab = 0;
    cursor_frame = 0;
    ko_move_cntdwn = ok_move_cntdwn = 0;
    game_state = 0;
    last_move_id = -1;
    win_anim_loops = win_anim_frames = 0;

    // Setting up palettes and graphics data for bg
    set_bkg_palette(0, 8, bg_palette);
    
    VBK_REG = 0;
    // Drawing board window
    for(uint8_t i=0; i<18; i++) set_bkg_based_tiles(0, i, 20, 1, blank_0, TILES_OFFSET);

    // Board borders
    for(uint8_t i=0; i<6; i++) for(uint8_t j=0; j<6; j++) set_bkg_tiles(4+2*i, 4+2*j, 2, 2, metasprite_tile);

    set_bkg_tiles(4, 3, 12, 1, takuzu_bg_top);
    set_bkg_tiles(4, 16, 12, 1, takuzu_bg_bot);

    set_bkg_tiles(0, 1, 4, 17, takuzu_bg_tubes_l0);    // Left tubes
    set_bkg_tiles(16, 2, 4, 16, takuzu_bg_tubes_r0);   // Right tubes
    // Text window
    draw_window(4, 0, 12, 3, true);

    // Setup visual board
    VBK_REG = 1;
    for(uint8_t i=0; i<18; i++) {
        if(i == 3 || i == 16) set_bkg_tiles(3, i, 14, 1, blank_3);
        else if(i < 4 || i > 16) set_bkg_tiles(3, i, 14, 1, blank_0);
        else set_bkg_tiles(3, i, 14, 1, takuzu_bg_center);
    }
    set_bkg_tiles(5, 1, 10, 1, blank_3);              // Text
    set_bkg_tiles(0, 0, 3, 18, takuzu_bg_tubes_l1);    // Left tubes
    set_bkg_tiles(17, 0, 3, 18, takuzu_bg_tubes_r1);   // Right tubes
    VBK_REG = 0;
    
    // Setting up sprites
    set_sprite_palette(0, 7, sprites_palette);              // Palettes
    gb_decompress_sprite_data(0, sprites_tiles_compressed); // Decompress sprites data

    cursor_setup();
    visual_tile_setup();
    setup_texts();

    // Setting up HUD
    setup_timer();

    // Setting up window
    VBK_REG = 0;
    for(uint8_t i=0; i<18; i++) set_win_tiles(0, i, 20, 1, blank_27);
    set_win_tiles(5, 3, 4, 1, pause_time);
    set_win_tiles(7, 5, 6, 2, pause_game_paused);
    draw_window(4, 2, 12, 6, false);
    set_win_tiles(6, 9, 8, 5, pause_text);
    VBK_REG = 1;
    for(uint8_t i=0; i<18; i++) set_win_tiles(0, i, 20, 1, blank_3);
    VBK_REG = 0;
    
    SHOW_SPRITES;
    SHOW_BKG;
    HIDE_WIN;
    DISPLAY_ON;

    // Ready screen
    show_text_ready();

    // Setting up board values
    setup_board(fixed_seed);
    if(fixed_seed && !first_gen) {
        for(uint8_t i = 30; i <= 36; i++) set_sprite_prop(i, 6);
        delay(500);
    } else if(first_gen) first_gen = false;

    // Go screen
    hide_text();
    show_text_go();
    setup_visual_board();
    play_sound_sel_valid();
    delay(1000);

    // Game starts
    load_puzzle_music();
    hide_text();
    update_visual_cursor();

}

void move_cursor() {
    dir = joypad() & 0x0F; // Direction only

    // Holding
    if(das_value && (old_dir == dir) && dir) {
        if(frames_held <= das_value) frames_held++;
        if(frames_held == das_value) { // Held for das_value frames
            if(mvt & J_LEFT) cursor_x = 0;
            else if(mvt & J_RIGHT) cursor_x = 5;
            if(mvt & J_UP) cursor_y = 0;
            else if(mvt & J_DOWN) cursor_y = 5;
            update_visual_cursor();
            play_sound_move();
        }
        return;
    }

    mvt = dir & (~old_dir);
    old_dir = dir;
    frames_held = 0;
    bool move = false;
    if(mvt & J_LEFT && cursor_x > 0) {
        cursor_x--;
        move = true;
    } else if(mvt & J_RIGHT && cursor_x < 5) {
        cursor_x++;
        move = true;
    }
    if(mvt & J_UP && cursor_y > 0) {
        cursor_y--;
        move = true;
    } else if(mvt & J_DOWN && cursor_y < 5) {
        cursor_y++;
        move = true;
    }
    if(move) {
        update_visual_cursor();
        play_sound_move();
    }
}

void cursor_animation() {
    if(cursor_frame == 30) {
        set_sprite_tile(0, 2);
        set_sprite_tile(1, 2);
        set_sprite_tile(2, 3);
        set_sprite_tile(3, 3);
    } else if(cursor_frame == 60) {
        cursor_frame = 0;
        set_sprite_tile(0, 0);
        set_sprite_tile(1, 0);
        set_sprite_tile(2, 1);
        set_sprite_tile(3, 1);
    }
}

void update_flags() {
    uint8_t last_cols, list_state, res, val;
    uint8_t i, j, axis;
    /*list state :
        bits :
        x....... : has 3 tiles of same color next to each other (1 = true, 0 = false)
        .x...... : has_gray (1 = true, 0 = false)
        ..xxx... : blue counter
        .....xxx : pink counter
    */

    for(axis=0; axis<2; axis++) {
        for(i=0; i<6; i++) {
            // Only change flags which contains tile & duplicates
            if((axis == 0 && i == cursor_x) || (axis == 1 && i == cursor_y) || (board_flags[axis][i] == 3)) {
                list_state = 0;
                last_cols = 0;
                for(j=0; j<6; j++) {
                    val = (axis==0) ? board[j][i] : board[i][j];
                    if(val & 1) list_state += 0x08;
                    else if(val & 2) list_state++;
                    else list_state = list_state | 0x40;

                    if(!(list_state & 0x80)) {
                        if(((last_cols >> 2) & 3) == (last_cols & 3) &&
                            (last_cols & 3) == (val & 3) &&
                            (last_cols & 3) != 0) {
                            // If last 3 are equal and not gray
                            list_state = list_state | 0x80;
                        }
                        last_cols = (last_cols << 2) | (val & 3);
                    }
                }

                if(list_state & 0x80) res = 2;  // KO
                else if((list_state & 0x07) > 3 || (list_state & 0x38) > 24) {  // >= 3 blues/pinks
                    res = 2;        // KO
                } else if((list_state & 0x07) != 3 || (list_state & 0x38) != 24) { // Test 3 blues & 3 pinks
                    // Test if gray tiles
                    if(list_state & 0x40) res = 0;  // Unfinished
                    else res = 2;  // KO
                } else {
                    uint8_t k;
                    res = 1;    // OK
                    for(j=0; j<6; j++) {
                        if(j != i) {
                            k = 0;
                            while(k<6) {
                                if((axis == 0 && (board[k][j] & 3) != (board[k][i] & 3)) ||
                                    axis == 1 && (board[j][k] & 3) != (board[i][k] & 3)   ) k = 255;
                                else k++;
                            }
                            if(k == 6) {
                                res = 3;    // KO : Found identical
                                board_flags[axis][j] = 3;
                                break;
                            }
                        }
                    }
                }
                board_flags[axis][i] = res;
            }
        }
    }
}

bool test_win() {
    for(uint8_t i=0; i<12; i++) {
            // Testing every flag result (if triple OK). Bitshift if unused bits are garbage
            if(*(board_flags[0] + i) != 01) return false;
    }
    return true;
}

bool action_change_tile() {
    tile = &board[cursor_y][cursor_x];
    // Tile is from init state
    if(*tile & 0xFC) return false;
    
    // Update moves list
    if(last_move_id < NB_MOVES_HIST - 1) last_move_id++;
    // If full, shift everything
    else for(uint8_t i=0; i<NB_MOVES_HIST - 1; i++) last_moves[i] = last_moves[i+1];
    // Add to the list
    last_moves[last_move_id] = (cursor_x << 5) | (cursor_y << 2) | (*tile);

    // Changing tile color
    *tile = (*tile + 1)%3;
    update_flags();
    update_visual_board();
    game_state = (game_state & 0xFE) | test_win();
    
    ok_move_cntdwn = 20;
    update_visual_tile();

    return true;
}

bool undo() {
    if(last_move_id < 0) return false; // No history
    uint8_t data = last_moves[last_move_id];
    cursor_x = data >> 5;
    cursor_y = (data >> 2) & 7;
    board[cursor_y][cursor_x] = data & 3;
    last_move_id--;
    update_flags();
    update_visual_cursor();
    update_visual_board();
    play_sound_move_ok();
    return true;
}

// Checks if player attempts to change tile
bool press_a_b() {
    ab = joypad() & 0x30; // AB only
    if(old_ab == ab) return true;

    delta_ab = ab & (~old_ab);
    old_ab = ab;

    if(delta_ab & J_A) {
        return action_change_tile();
    } else if(delta_ab & J_B) {
        return undo();
    }

    return true;
}

void press_pause() {
    if(!(joypad() & (J_START | J_SELECT))) return;
    game_state = pause();
    waitpadup();
}

void flicker_palettes(bool alternate) {
    for(uint8_t i=0; i < FLICKER_ENTRY_NB; i++) {
        uint16_t* temp = flicker_palettes_values[i];
        set_bkg_palette_entry(
            temp[0] >> 8,
            temp[0] & 0xFF,
            alternate ? temp[2] : temp[1]);
    }
}

void update_win_anim() {
    win_anim_frames++;
    if(win_anim_frames == 30) {
        flicker_palettes(true);
    }
    if(win_anim_frames == 60) {
        win_anim_frames = 0;
        if(win_anim_loops < 3) win_anim_loops++;
        flicker_palettes(false);
    }
}

// Game screen
uint16_t game_screen(uint8_t das, bool fixed_seed) {
    DISPLAY_OFF;
    first_gen = true;
    das_value = das;
    // If using code, only generates the board corresponding
    while(1) {
        game_setup(fixed_seed);

        // GAME LOOP
        while(1) {
            cursor_frame++;
            cursor_animation();
            move_cursor();
            ok_move = press_a_b();
            if(!ok_move && !ko_move_cntdwn) ko_move_cntdwn = 32;
            ko_move_cursor_anim();
            update_visual_tile();
            update_timer();
            if(!(game_state & 1)) press_pause();
            if(game_state) break; // End of loop (Reset, win or back to menu)
            wait_vbl_done();    // Wait end of frame
            play_music();
        }
        
        if(game_state & 0b101) break; // Player won or back to menu
        // New game : nothing to be done
    }
    if(game_state & 1) {    // Player won
        update_visual_timer(true);
        // Remove sprites & load music
        HIDE_SPRITES;
        load_win_music();
        uint8_t music_finished = false;
        uint16_t timeout = 0;   // in case music is off
        while(1) {
            update_win_anim();

            if(((win_anim_loops > 2) && joypad()) || (music_finished) || (timeout == 900)) {
                waitpadup();
                DISPLAY_OFF;
                return time();
            }

            wait_vbl_done();
            timeout++;
            if(win_anim_loops || win_anim_frames > 30) {
                music_finished = play_music();
            }
        }
    }
    return 0;
}