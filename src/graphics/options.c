#include "options.h"
#include "font.h"

#define __ 27

const uint8_t options_text[] =
{
    18,  3, __, __, __,
    __, __, __, __, __,
     D,  I,  F,  F, __,
    __, __, __, __, __,
    18, 14, 20, 13,  3,
    __, __, __, __, __,
    12, 20, 18,  8,  2,
    __, __, __, __, __,
    12,  4, 13, 20, __
};

const uint8_t options_text_das[] =
{
    __, S, H, I, F, T,__, D, E, L, A, Y,__,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,
    28,__, T, O,__, T, U, R, N,__, O, F, F,__,
    37,__, F, O, R,__, M, A, X, I, M, U, M,__,
};

const uint8_t options_text_diff[] =
{
    __,__, D, I, F, F, I, C, U, L, T, Y,__,__,
    __,__,__,__,__,__,__,__,__,__,__,__,__,__,
     H, A, R, D,__, M, O, D, E,__, A, D, D, S,
     D, E, D, U, C, I, N, G,__, R, U, L, E, S,
};

const uint8_t options_text_diff_list[] = {E, A, S, Y, H, A, R, D};

const uint8_t options_on[] = {14, 13, __};
const uint8_t options_off[] = {14, 5, 5};