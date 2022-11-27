#include <gb/gb.h>
#include <stdbool.h>
#include <stdint.h>
#include "audio.h"
#include "music.h"

bool sound_state = true;
bool music_state = true;

// Register values of various sounds for channel 2
uint8_t reg_values_2[] = {
    0x80, 0xF1, 0x06, 0x87,   //sel & move ok
    0x80, 0xF1, 0x73, 0x87,   //sel_valid
    0x80, 0xD1, 0x90, 0x87,   //char
    0x80, 0x91, 0x9E, 0x86,   //move
    0x80, 0xF1, 0x62, 0x87,   //pause p1
    0x80, 0xF2, 0x6B, 0x87,   //pause p2
};

// Register values of various sounds for channel 1
uint8_t reg_values_1[] = {
    0x7A, 0x40, 0xF2, 0x28, 0x85,   //move_ko
};

// Puts on audio output
void sound_on() {
    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;
}

void sound_off() {
    NR52_REG = 0x00;
}

void update_audio() {
    if(!(sound_state | music_state)) sound_off();
    else sound_on();
}

bool sound_value() {return sound_state;}

bool music_value() {return music_state;}

void switch_sound() {
    sound_state = !sound_state;
    update_audio();
}
void switch_music() {
    music_state = !music_state;
    update_audio();
}

void load_channel_1(uint8_t *values) {
    if(!sound_state) return;
    NR10_REG = values[0];
    NR11_REG = values[1];
    NR12_REG = values[2];
    NR13_REG = values[3];
    NR14_REG = values[4];
}

void load_channel_2(uint8_t *values) {
    if(!sound_state) return;
    NR21_REG = values[0];
    NR22_REG = values[1];
    NR23_REG = values[2];
    NR24_REG = values[3];
}

void play_sound_sel()         {load_channel_2(&reg_values_2[0]);}
void play_sound_sel_valid()   {load_channel_2(&reg_values_2[4]);}
void play_sound_char()        {load_channel_2(&reg_values_2[8]);}
void play_sound_move_ok()     {load_channel_2(&reg_values_2[0]);}
void play_sound_move()        {load_channel_2(&reg_values_2[12]);}
void play_sound_move_ko()     {load_channel_1(&reg_values_1[0]);}
void play_sound_pause_1()     {load_channel_2(&reg_values_2[16]);}
void play_sound_pause_2()     {load_channel_2(&reg_values_2[20]);}

void load_puzzle_music() {
    if(music_state) load_music(puzzle_music, puzzle_wave_pattern, 128, 324, true);
}

void load_win_music() {
    if(music_state) load_music(win_music, win_wave_pattern, 48, 300, false);
}

bool play_music() {
    if(music_state) return update_music();
    return false;
}