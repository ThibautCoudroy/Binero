#include <gb/gb.h>
#include <stdint.h>
#include <stdbool.h>
#include "timer.h"

uint8_t time_sec, time_min;
uint16_t time_frame;

unsigned char text[] = {'t'-'a', 'i'-'a', 'm'-'a', 'e'-'a', '|'-'a'};    //"time "

// drawing bkg (true) or win (false)
void update_visual_timer(bool bkg) {
    // Updating values and display
    if(time_min > 0) {
        signed char time_values[] = {time_min / 10, time_min % 10, 11, time_sec / 10, time_sec % 10};
        // Update background & window 
        if(bkg) set_bkg_based_tiles(10, 1, 5, 1, time_values, 28);
        else set_win_based_tiles(10, 3, 5, 1, time_values, 28);
        VBK_REG = 1;
        if(bkg) {
            set_bkg_tile_xy(13, 1, 3);
            set_bkg_tile_xy(14, 1, 3);
        } else {
            set_win_tile_xy(13, 3, 3);
            set_win_tile_xy(14, 3, 3);
        }
        VBK_REG = 0;
    } else {
        uint8_t ticks = ((uint16_t) (time_frame % 60) * 5) / 3;
        signed char time_values[] = {time_sec / 10, time_sec % 10, 12,  ticks / 10 + 13, ticks % 10 + 13};
        // Update background & window
        if(bkg) set_bkg_based_tiles(10, 1, 5, 1, time_values, 28);
        else {
            time_values[4] = 13;
            set_win_based_tiles(10, 3, 5, 1, time_values, 28);
        }
    }
}

uint16_t time() {
    return time_frame;
}

void setup_timer() {
    time_sec = time_min = time_frame = 0;
    set_bkg_tiles(5, 1, 5, 1, text);
    update_visual_timer(true);
}

void update_timer() {
    if(time_min == 18 && time_sec == 0) return;    // avoid overflow
    time_frame++;
    if((time_frame % 60) == 0) {
        time_sec++;
        if(time_sec == 60) {
            time_sec = 0;
            time_min++;
        }
        if(time_min) {
            update_visual_timer(true);
            return;
        }
    }

    if(!(time_min | time_frame % 6)) update_visual_timer(true);
}