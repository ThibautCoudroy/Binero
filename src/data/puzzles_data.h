#include <stdint.h>

// Each puzzle is 2.5 bytes long (5 bytes = 2 puzzles)
// Each half-byte contains value between 0 and 13 included corresponding to which line possibility it belongs to
// Puzzle 1 : 11112222 33334444 5555xxxx xxxxxxxx xxxxxxxx
// Puzzle 2 : xxxxxxxx xxxxxxxx xxxx1111 22223333 44445555

extern uint8_t puzzle_data[512][5];