/* Credits to scazon for parts of code and frequencies */

#include <stdint.h>
#include "music_data.h"

#define __ 0
#define ____ 0
#define FULL 0x20
#define HALF 0x40
#define _QRT 0x60

//Define note names
enum {
    C3, Cd3, D3, Dd3, E3, F3, Fd3, G3, Gd3, A3, Ad3, B3,
    C4, Cd4, D4, Dd4, E4, F4, Fd4, G4, Gd4, A4, Ad4, B4,
    C5, Cd5, D5, Dd5, E5, F5, Fd5, G5, Gd5, A5, Ad5, B5,
    C6, Cd6, D6, Dd6, E6, F6, Fd6, G6, Gd6, A6, Ad6, B6,
    C7, Cd7, D7, Dd7, E7, F7, Fd7, G7, Gd7, A7, Ad7, B7,
    C8, Cd8, D8, Dd8, E8, F8, Fd8, G8, Gd8, A8, Ad8, B8,
};

enum instruments {
    NONE, SINE, CHIP, DRM1,
};

uint8_t puzzle_wave_pattern[] =
{
    136, 136, 135, 101, 68, 51, 68, 103, 137, 172, 205, 220, 187, 169, 136, 136, 
};

uint8_t win_wave_pattern[] =
{
    137, 171, 187, 169, 153, 171, 187, 169, 134, 84, 68, 86, 102, 84, 68, 86, 
};

uint8_t puzzle_music[] = {
    // CHAN 1 & 2 : instrument, pitch, enveloppe
    // CHAN 3 : pitch, volume
    // CHAN 4 : instrument, enveloppe

    // CHANNEL 1 -- CHANNEL 2 ----- CHANNEL 3 CHANNEL 4
    CHIP, D6, 0x83, NONE, __, ____, D4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    CHIP, D6, 0x83, NONE, __, ____, D4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, F6, 0x83, NONE, __, ____, F4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    CHIP, F6, 0x82, NONE, __, ____, F4, FULL, NONE, ____,
    CHIP, E6, 0x82, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, D6, 0x82, NONE, __, ____, D4, FULL, NONE, ____,
    CHIP, E6, 0x83, NONE, __, ____, E4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,

    CHIP, C6, 0x83, NONE, __, ____, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    CHIP, C6, 0x83, NONE, __, ____, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, C6, 0x83, NONE, __, ____, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    CHIP, G5, 0x82, NONE, __, ____, C4, FULL, NONE, ____,
    CHIP, A5, 0x82, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, B5, 0x82, NONE, __, ____, B3, FULL, NONE, ____,
    CHIP, A5, 0x83, NONE, __, ____, A3, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, D5, 0x82, NONE, __, ____, __, ____, DRM1, 0x71,
// 3
    CHIP, D6, 0x83, NONE, __, ____, D4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    CHIP, D6, 0x83, NONE, __, ____, D4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, F6, 0x83, NONE, __, ____, F4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    CHIP, F6, 0x82, NONE, __, ____, F4, FULL, NONE, ____,
    CHIP, E6, 0x82, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, D6, 0x82, NONE, __, ____, D4, FULL, NONE, ____,
    CHIP, E6, 0x82, NONE, __, ____, E4, FULL, NONE, ____,
    SINE, E6, 0x83, NONE, __, ____, __, ____, NONE, ____,
    SINE, D6, 0x83, NONE, __, ____, __, ____, DRM1, 0x71,
    SINE, E6, 0x83, NONE, __, ____, __, ____, DRM1, 0x71,

    CHIP, C6, 0x83, NONE, __, ____, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    CHIP, C6, 0x83, NONE, __, ____, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, C6, 0x83, NONE, __, ____, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    CHIP, G5, 0x82, NONE, __, ____, C4, FULL, NONE, ____,
    CHIP, A5, 0x82, NONE, __, ____, __, ____, DRM1, 0x71,
    CHIP, B5, 0x82, NONE, __, ____, B3, FULL, NONE, ____,
    CHIP, A5, 0x82, NONE, __, ____, A3, FULL, NONE, ____,
    SINE, A6, 0x83, NONE, __, ____, __, ____, NONE, ____,
    SINE, G6, 0x83, NONE, __, ____, __, ____, DRM1, 0x71,
    SINE, A6, 0x83, NONE, __, ____, __, ____, DRM1, 0x71,
// 5
    SINE, D6, 0x84, SINE, D5, 0x64, D4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, D6, 0x84, SINE, D5, 0x64, D4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, D6, 0x83, SINE, D5, 0x63, __, ____, DRM1, 0x71,
    SINE, F6, 0x84, SINE, F5, 0x64, F4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, F6, 0x83, SINE, F5, 0x63, F4, FULL, NONE, ____,
    SINE, E6, 0x83, SINE, E5, 0x63, __, ____, DRM1, 0x71,
    SINE, D6, 0x83, SINE, D5, 0x63, D4, FULL, NONE, ____,
    SINE, E6, 0x84, SINE, E5, 0x64, E4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, E6, 0x63, SINE, E5, 0x43, __, ____, DRM1, 0x71,
    SINE, E6, 0x63, SINE, E5, 0x43, __, ____, DRM1, 0x71,

    SINE, C6, 0x84, SINE, C5, 0x64, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, C6, 0x84, SINE, C5, 0x64, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, A5, 0x83, SINE, A4, 0x63, __, ____, DRM1, 0x71,
    SINE, C6, 0x84, SINE, C5, 0x64, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, A5, 0x83, SINE, A4, 0x63, C4, FULL, NONE, ____,
    SINE, F5, 0x83, SINE, F4, 0x63, __, ____, DRM1, 0x71,
    SINE, E5, 0x83, SINE, E4, 0x63, B3, FULL, NONE, ____,
    SINE, D5, 0x84, SINE, D4, 0x64, A3, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, D5, 0x63, SINE, D4, 0x43, __, ____, DRM1, 0x71,
    SINE, D5, 0x63, SINE, D4, 0x43, __, ____, DRM1, 0x71,
// 7
    SINE, D6, 0x84, SINE, D5, 0x64, D4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, D6, 0x84, SINE, D5, 0x64, D4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, D6, 0x83, SINE, D5, 0x63, __, ____, DRM1, 0x71,
    SINE, F6, 0x84, SINE, F5, 0x64, F4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, F6, 0x83, SINE, F5, 0x63, F4, FULL, NONE, ____,
    SINE, E6, 0x83, SINE, E5, 0x63, __, ____, DRM1, 0x71,
    SINE, D6, 0x83, SINE, D5, 0x63, D4, FULL, NONE, ____,
    SINE, E6, 0x84, SINE, E5, 0x64, E4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, E6, 0x63, SINE, E5, 0x43, __, ____, DRM1, 0x71,
    SINE, E6, 0x63, SINE, E5, 0x43, __, ____, DRM1, 0x71,

    SINE, C6, 0x84, SINE, C5, 0x64, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, C6, 0x84, SINE, C5, 0x64, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, A5, 0x83, SINE, A4, 0x63, __, ____, DRM1, 0x71,
    SINE, C6, 0x84, SINE, C5, 0x64, C4, FULL, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, A5, 0x83, NONE, __, ____, A3, HALF, NONE, ____,
    SINE, F5, 0x83, NONE, __, ____, F3, HALF, DRM1, 0x71,
    SINE, E5, 0x83, NONE, __, ____, E3, HALF, NONE, ____,
    SINE, D5, 0x84, NONE, __, ____, D3, HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
};

uint8_t win_music[] = {
    // CHAN 1 & 2 : instrument, pitch, enveloppe
    // CHAN 3 : pitch, volume
    // CHAN 4 : instrument, enveloppe

    // CHANNEL 1 -- CHANNEL 2 ----- CHANNEL 3 CHANNEL 4
// 1
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    SINE, Fd5,0xA3, SINE, Fd4,0x53, Fd3,HALF, NONE, ____,
    SINE, Ad5,0xA3, SINE, Ad4,0x53, Dd4,HALF, NONE, ____,
    SINE, Dd6,0xA3, SINE, Dd5,0x53, Dd4,HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    SINE, Fd5,0xA3, SINE, Fd4,0x53, Fd3,HALF, NONE, ____,
    SINE, Ad5,0xA3, SINE, Ad4,0x53, A3, HALF, NONE, ____,
    SINE, A5, 0xA3, SINE, A4, 0x53, A3, HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
// 2
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    SINE, Fd5,0xA3, SINE, Fd4,0x53, __, ____, DRM1, 0x71,
    SINE, Ad5,0xA3, SINE, Ad4,0x53, __, ____, NONE, ____,
    SINE, Dd6,0xA3, SINE, Dd5,0x53, __, ____, NONE, ____,
    NONE, __, ____, SINE, D5, 0x53, __, ____, NONE, ____,
    NONE, __, ____, SINE, C5, 0x53, C4, _QRT, DRM1, 0x71,
    NONE, __, ____, SINE, Ad4,0x53, Ad3,HALF, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    SINE, Fd5,0xA3, SINE, Fd4,0x53, __, ____, DRM1, 0x71,
    SINE, Ad5,0xA3, SINE, Ad4,0x53, __, ____, NONE, ____,
    SINE, Cd6,0xA3, SINE, Cd5,0x53, __, ____, NONE, ____,
    NONE, __, ____, SINE, C5, 0x53, __, ____, NONE, ____,
    NONE, __, ____, SINE, Ad4,0x53, Ad3,_QRT, DRM1, 0x71,
    NONE, __, ____, SINE, Gd4,0x53, Gd3,HALF, NONE, ____,
// 3
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    SINE, Fd5,0xA3, SINE, Fd4,0x53, Fd3,HALF, NONE, ____,
    SINE, Ad5,0xA3, SINE, Ad4,0x53, Dd4,HALF, NONE, ____,
    SINE, Dd6,0xA3, SINE, Dd5,0x53, Dd4,HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    SINE, C6, 0xA3, SINE, C5, 0x53, C4, HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    SINE, Fd5,0xA3, SINE, Fd4,0x53, Fd3,HALF, NONE, ____,
    SINE, Ad5,0xA3, SINE, Ad4,0x53, A3, HALF, NONE, ____,
    SINE, A5, 0xA3, SINE, A4, 0x53, A3, HALF, NONE, ____,
    NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
// Other ver
    // CHIP, C6, 0xA3, NONE, __, ____, C4, HALF, NONE, ____,
    // CHIP, C6, 0xA3, NONE, __, ____, C4, HALF, NONE, ____,
    // NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    // CHIP, Fd5,0xA3, NONE, __, ____, Fd3,HALF, NONE, ____,
    // CHIP, Ad5,0xA3, NONE, __, ____, Dd4,HALF, NONE, ____,
    // CHIP, Dd6,0xA3, NONE, __, ____, Dd4,HALF, NONE, ____,
    // NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    // NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    // CHIP, C6, 0xA3, NONE, __, ____, C4, HALF, NONE, ____,
    // CHIP, C6, 0xA3, NONE, __, ____, C4, HALF, NONE, ____,
    // NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    // CHIP, Fd5,0xA3, NONE, __, ____, Fd3,HALF, NONE, ____,
    // CHIP, Ad5,0xA3, NONE, __, ____, A3, HALF, NONE, ____,
    // CHIP, A5, 0xA3, NONE, __, ____, A3, HALF, NONE, ____,
    // NONE, __, ____, NONE, __, ____, __, ____, DRM1, 0x71,
    // NONE, __, ____, NONE, __, ____, __, ____, NONE, ____,
    
};
