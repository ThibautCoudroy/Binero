#include <stdbool.h>
#include "music_data.h"

// Loads music and info
void load_music(uint8_t music_data[], uint8_t wave_pattern[], uint8_t b_nb, uint16_t bpm, bool loop);

// Calls every frame to update audio. Return true if audio has ended
bool update_music();