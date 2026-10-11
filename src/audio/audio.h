#include <stdint.h>
#include <stdbool.h>

extern void sound_on(void);
extern void sound_off(void);
extern bool sound_value(void);
extern bool music_value(void);
extern void switch_sound(void);
extern void switch_music(void);
extern void play_sound_sel(void);
extern void play_sound_sel_valid(void);
extern void play_sound_char(void);
extern void play_sound_move_ok(void);
extern void play_sound_move_ko(void);
extern void play_sound_move(void);

extern void play_sound_pause_1(void);
extern void play_sound_pause_2(void);

extern void load_puzzle_music(void);
extern void load_win_music(void);
extern bool play_music(void);