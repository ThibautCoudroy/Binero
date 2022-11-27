/* Credits to scazon for frequencies */

#include <stdint.h>
#include <stdbool.h>
#include <gb/gb.h>
#include "music.h"
#include "music_data.h"

enum instruments {
    NONE, SINE, CHIP, DRM1,
};

uint8_t beat, beats_nb, frames, frames_delta;
bool loop;
bool hasStarted;
uint8_t* music_data;
uint8_t* wave_pattern_pointer = 0xff30;

const int16_t frequencies[] = { //values based on a formula used by the GB processor
    44, 156, 262, 363, 457, 547, 631, 710, 786, 854, 923, 986,
    1046, 1102, 1155, 1205, 1253, 1297, 1339, 1379, 1417, 1452, 1486, 1517,
    1546, 1575, 1602, 1627, 1650, 1673, 1694, 1714, 1732, 1750, 1767, 1783,
    1798, 1812, 1825, 1837, 1849, 1860, 1871, 1881, 1890, 1899, 1907, 1915,
    1923, 1930, 1936, 1943, 1949, 1954, 1959, 1964, 1969, 1974, 1978, 1982,
    1985, 1988, 1992, 1995, 1998, 2001, 2004, 2006, 2009, 2011, 2013, 2015,
    0,
};

void play_note() {
    uint8_t* n = &music_data[beat * 10];

    // CHANNEL 1
    switch(n[0]) {
        case SINE:
            NR10_REG = 0x00;
            NR11_REG = 0xB0;
            NR12_REG = n[2];
            NR13_REG = (uint8_t) frequencies[n[1]];
            NR14_REG = 0xC0 | (frequencies[n[1]] >> 8);
            break;
        case CHIP:
            NR10_REG = 0x00;
            NR11_REG = 0x70;
            NR12_REG = n[2];
            NR13_REG = (uint8_t) frequencies[n[1]];
            NR14_REG = 0xC0 | (frequencies[n[1]] >> 8);
            break;
    }

    // CHANNEL 2
    switch(n[3]) {
        case SINE:
            NR21_REG = 0x80;
            NR22_REG = n[5];
            NR23_REG = (uint8_t) frequencies[n[4]];
            NR24_REG = 0x80 | (frequencies[n[4]] >> 8);
            break;
    }

    // CHANNEL 3
    NR30_REG = 0x80;
    NR31_REG = 0xD0;
    NR32_REG = n[7];
    NR33_REG = (uint8_t) frequencies[n[6]];
    NR34_REG = 0xC0 | (frequencies[n[6]] >> 8);

    // CHANNEL 4
    switch(n[8]) {
        case DRM1:
            NR41_REG = 0x00;
            NR42_REG = n[9];
            NR43_REG = 0x35;
            NR44_REG = 0xC0;
            break;
    }
}

void load_music(uint8_t m_data[], uint8_t w_data[], uint8_t b_nb, uint16_t bpm, bool l) {
    music_data = m_data;
    beats_nb = b_nb;
    loop = l;
    frames_delta = 3600 / bpm;
    hasStarted = false;
    beat = beats_nb - 1;
    frames = 0;

    // Setup wave pattern
    NR30_REG = 0;
    for(uint8_t i=0; i<16; i++) *(wave_pattern_pointer + i) = *(w_data +  i);

    play_note();    // Loads correctly song (first call doesn't make any sound)
}

bool update_music() {
    frames++;
    if(frames == frames_delta) {
        frames = 0;
        beat++;
        if(beats_nb == beat) {  // End reached
            beat = 0;
            if(!loop & hasStarted) return true;
            hasStarted = true;
        }
        play_note();
    }
    return false;
}