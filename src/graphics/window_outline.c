#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include "window_outline.h"

void draw_window(uint8_t x, uint8_t y, uint8_t w, uint8_t h, bool bkg){
    // pointing to function depending on bkg or win
    uint8_t * (*set_tile)(uint8_t, uint8_t, uint8_t) = bkg ? set_bkg_tile_xy : set_win_tile_xy;
    uint8_t temp = y+h-1;
    uint8_t temp2 = x+w-1;
    
    set_tile(x, y, 76);
    set_tile(x+1, y, 70);
    set_tile(x, y+1, 73);
    set_tile(temp2, y, 77);
    set_tile(temp2, y+1, 75);
    set_tile(x, temp, 79);
    set_tile(x+1, temp, 72);
    set_tile(temp2, temp, 78);

    for(uint8_t i=x+2; i<temp2; i++) {
        set_tile(i, y, 71);
        set_tile(i, temp, 71);
    }
    for(uint8_t i=y+2; i<temp; i++) {
        set_tile(x, i, 74);
        set_tile(temp2, i, 74);
    }
}