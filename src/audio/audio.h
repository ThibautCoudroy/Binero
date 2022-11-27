#include <stdint.h>
#include <stdbool.h>

extern uint8_t reg_values[];

extern void sound_on();
extern void sound_off();
extern bool sound_value();
extern bool music_value();
extern void switch_sound();
extern void switch_music();
extern void play_sound_sel();
extern void play_sound_sel_valid();
extern void play_sound_char();
extern void play_sound_move_ok();
extern void play_sound_move_ko();
extern void play_sound_move();

extern void play_sound_pause_1();
extern void play_sound_pause_2();

extern void load_puzzle_music();
extern void load_win_music();
extern bool play_music();