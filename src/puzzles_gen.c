#include <stdbool.h>
#include <stdint.h>
#include <rand.h>
#include <gb/gb.h>
#include "puzzles_gen.h"
#include "data/puzzles_data.h"

#define NB_TILES_REMOVED 4
#define NB_UNDO_MAX 8

uint8_t tiles_to_remove;    // Number of tiles removed each step
uint8_t nb_undo;            // Number of times a necessary tile was removed
uint8_t progress;
uint8_t board_temp[6][6];
uint8_t board_statement[6][6];
uint8_t removed_history[NB_TILES_REMOVED][3];
bool solve_board(bool modif);

uint8_t lines_poss[][6] = {
    {1, 1, 2, 1, 2, 2},
    {1, 1, 2, 2, 1, 2},
    {1, 2, 1, 1, 2, 2},
    {1, 2, 1, 2, 1, 2},
    {1, 2, 1, 2, 2, 1},
    {1, 2, 2, 1, 1, 2},
    {1, 2, 2, 1, 2, 1},

    {2, 2, 1, 2, 1, 1},
    {2, 2, 1, 1, 2, 1},
    {2, 1, 2, 2, 1, 1},
    {2, 1, 2, 1, 2, 1},
    {2, 1, 2, 1, 1, 2},
    {2, 1, 1, 2, 2, 1},
    {2, 1, 1, 2, 1, 2}
};

void get_valid_sol() {
    uint8_t temp_board[6][6];
    uint8_t temp_lines[5];                  // 6th line is deduced from the other
    uint16_t board_id = randw() & 0x01FF;   // 512 pairs of boards
    uint8_t rand_val = rand() & 1;          // indicates first or second board of the pair
    
    // Decode puzzle data
    for(uint8_t i = 0, j = rand_val * 5; i < 5; i++, j++) {
        temp_lines[i] = puzzle_data[board_id][j/2];
        if(j&1) temp_lines[i] &= 0x0F;
        else    temp_lines[i] >>= 4;
    }

    // 50% chance of colors swap
    if(rand()&1) {
        // Shift 7 from each line possibility
        for(uint8_t i=0; i<6; i++) {
            temp_lines[i] = (temp_lines[i] + 7)%14;
        }
    }

    // 50% chance of copying mirror
    rand_val = rand()&1;
    for(uint8_t l=0; l<5; l++) {
        for(uint8_t j=0; j<6; j++) {
            temp_board[l][j] = lines_poss[temp_lines[l]][(rand_val ? 5-j : j)];     // Copy the line into temp board
            temp_board[5][j] = 0;   // Last line is empty because it's gonna be deduced
        }
    }

    // 25% chance for every rotation
    rand_val = rand()&3;
    uint8_t t1, t2;
    for(uint8_t k=0; k<6; k++) {
        for(uint8_t m=0; m<6; m++) {
            if(rand_val == 0) {
                t1 = k;
                t2 = m;
            } else if(rand_val == 1) {
                t1 = m;
                t2 = 5-k;
            } else if(rand_val == 2) {
                t1 = 5-k;
                t2 = 5-m;
            } else {
                t1 = 5-m;
                t2 = k;
            }

            board_statement[k][m] = temp_board[t1][t2];
        }
    }

    // Solve last 6 cases
    solve_board(true);
}

// Remove "tiles_to_remove" tiles of the board
void remove_tiles() {
    uint8_t nb_removed = 0;
    while(nb_removed < tiles_to_remove) {
        // Replacing a tile by "0"
        uint8_t x = rand()%6;
        uint8_t y = rand()%6;
        if(board_statement[x][y] != 0) {
            // Add to history
            removed_history[nb_removed][0] = x;
            removed_history[nb_removed][1] = y;
            removed_history[nb_removed][2] = board_statement[x][y];

            // Remove
            board_statement[x][y] = 0;
            nb_removed++;
        }
    }
}

// Undo tiles removed at last step
void undo_remove_tiles() {
    for(uint8_t i=0; i < tiles_to_remove; i++) {
        uint8_t* temp = removed_history[i];
        board_statement[temp[0]][temp[1]] = temp[2];
    }
}

// Tries to solve board_statement (usage of board_temp to solve. If update_board_statement = true, copies back to board_statement)
bool solve_board(bool update_board_statement) {
    for(uint8_t k=0; k<36; k++) *(board_temp[0] + k) = *(board_statement[0] + k);   // Temp copies statement
    
    uint8_t list1[6], list2[6];
    uint8_t incr, val;
    bool modif, hasZero;

    // Tries to fill every tile until nothing more to do
    do {
        modif = false;
        for(uint8_t axis = 0; axis < 2; axis++) {
            for(uint8_t i = 0; i < 6; i++) {
                hasZero = false;

                // Copy line/column
                for(incr=0; incr<6; incr++) {
                    val = *(board_temp[0] + (axis ? 6*incr + i : incr + 6*i));
                    if(val == 0) hasZero = true;
                    list1[incr] = val;
                }
                // If not completed
                if(hasZero) {
                    // Count colors
                    uint8_t cnt[3] = {0, 0, 0};
                    for(incr=0; incr<6; incr++) cnt[list1[incr]]++;
                    // If color completed, finish the other
                    for(uint8_t col=1; col<3; col++) {
                        if(cnt[col] == 3) {
                            for(incr=0; incr<6; incr++) {
                                if(list1[incr] == 0) list1[incr] = 3-col;
                            }
                            modif = true;
                            break;
                        }
                    }

                    for(incr=0; incr<5; incr++) {
                        uint8_t temp_val = list1[incr];
                        // Complete if two of the same color are next to eachother
                        if(temp_val && (temp_val == list1[incr+1])) {
                            if(incr <= 3 && !list1[incr+2]) {
                                list1[incr+2] = 3-temp_val;
                                modif = true;
                            }
                            if(incr >= 1 && !list1[incr-1]) {
                                list1[incr-1] = 3-temp_val;
                                modif = true;
                            }
                        // Complete if two of the same color separated by blank
                        } else if(incr < 4 && temp_val && temp_val == list1[incr+2] && !list1[incr+1]) {
                            list1[incr+1] = 3-temp_val;
                            modif = true;
                        }
                    }

                    // Complete if two '0' left and another line has 4 in common
                    uint8_t cnt_0 = 6;
                    for(incr=0; incr<6; incr++) {
                        if(*(board_temp[0] + (axis ? 6*incr + i : incr + 6*i))) cnt_0--;
                    }
                    if(cnt_0 == 2) {    // Two '0' left
                        for(uint8_t j=i+1; j<6; j++) {
                            
                            hasZero = false;

                            // List2 copies the other line/column
                            for(incr=0; incr<6; incr++) {
                                val = *(board_temp[0] + (axis ? 6*incr + j : incr + 6*j));
                                if(val == 0) {
                                    hasZero = true;
                                    break;
                                }
                                list2[incr] = val;
                            }
                            if(!hasZero) {
                                // Check if common
                                uint8_t sum = 0;
                                for(incr=0; incr<6; incr++) {
                                    if(list1[incr] == list2[incr]) sum++;
                                }

                                if(sum == 4) {
                                    for(incr=0; incr<6; incr++) {
                                        if(!list1[incr]) list1[incr] = 3 - list2[incr];
                                    }
                                    modif = true;
                                    break;
                                }
                            }
                        }
                    }

                    // Writing the new line/column in the grid
                    for(incr=0; incr<6; incr++) {
                        *(board_temp[0] + (axis ? 6*incr + i : incr + 6*i)) = list1[incr];
                    }
                }
            }
        }
    } while(modif);

    // In case modif = true, should be solved. Statement copies back temp
    if(update_board_statement) for(uint8_t g=0; g<36; g++) *(board_statement[0] + g) = *(board_temp[0] + g);

    // If there's still a 0, then they're at least 2 solutions
    for(uint8_t z=0; z<36; z++) {
        if(*(board_temp[0] + z) == 0) return false;
    }
    return true;
}

void update_loading_bar() {
    progress++;
    set_sprite_prop(progress + 29, 6);
}

void generate_board(uint8_t board[6][6]) {
    progress = 0;
    // STEP 1 : BOARD_STATEMENT GETS A VALID SOLUTION
    get_valid_sol();

    // STEP 2 : REMOVE TILES FROM STATEMENT
    tiles_to_remove = NB_TILES_REMOVED;
    nb_undo = 0;
    
    while(nb_undo < NB_UNDO_MAX) {
        remove_tiles();
        // If not solvable anymore, we undo
        if(!solve_board(false)) {
            undo_remove_tiles();
            nb_undo++;
            update_loading_bar();
            if(tiles_to_remove != 1) tiles_to_remove--;  // Removing tiles more slowly
        }
    }

    // STEP 3 : BOARD COPIES BOARD_STATEMENT
    for(uint8_t k=0; k<36; k++) {
        uint8_t t = *(board_statement[0] + k);
        *(board[0] + k) = t ? t+4 : 0;
    }
}

/*
Board to try out every rule
    {0,0,5,5,0,0},
    {0,0,0,0,5,0},
    {0,6,0,5,0,0},
    {5,0,0,5,0,5},
    {0,0,0,0,0,0},
    {0,0,0,0,0,6},
*/

// uint8_t puzzle[][6][6] = {
//     {{5, 6, 0, 5, 6, 6},
//     {6, 5, 6, 6, 0, 0},
//     {6, 5, 0, 5, 0, 0},
//     {5, 6, 0, 6, 6, 0},
//     {0, 0, 0, 5, 6, 0},
//     {6, 6, 5, 0, 5, 5}}
// };

/*uint8_t puzzle[10][6][6] = {
    {{0, 0, 0, 6, 0, 0},
    {0, 0, 0, 0, 0, 5},
    {0, 0, 0, 6, 0, 0},
    {5, 0, 0, 0, 0, 5},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 6, 6, 0, 0}
    },
    {{0, 0, 0, 6, 0, 0},
    {0, 0, 0, 5, 0, 0},
    {5, 0, 0, 0, 0, 0},
    {5, 5, 0, 6, 0, 0},
    {0, 5, 0, 0, 5, 5},
    {6, 0, 0, 0, 0, 0}
    },
    {{0, 5, 0, 5, 0, 0},
    {0, 0, 0, 0, 5, 0},
    {5, 5, 0, 6, 5, 0},
    {5, 0, 5, 0, 0, 0},
    {0, 0, 0, 6, 0, 0},
    {0, 0, 6, 0, 0, 0}
    },
    {{0, 0, 0, 0, 0, 0},
    {0, 5, 0, 5, 0, 0},
    {0, 5, 0, 5, 6, 0},
    {0, 0, 5, 0, 6, 0},
    {0, 0, 0, 0, 0, 0},
    {5, 0, 5, 0, 0, 0}
    },
    {{5, 6, 0, 5, 6, 6},
    {6, 5, 6, 6, 0, 0},
    {6, 5, 0, 5, 0, 0},
    {5, 6, 0, 6, 6, 0},
    {0, 0, 0, 5, 6, 0},
    {6, 6, 5, 0, 5, 5}
    },
    {{6, 5, 0, 0, 0, 0},
    {0, 0, 6, 6, 0, 5},
    {0, 0, 0, 0, 5, 0},
    {5, 0, 5, 0, 0, 0},
    {0, 0, 0, 0, 0, 0},
    {0, 0, 5, 5, 0, 6}
    },
    {{0, 0, 0, 0, 0, 0},
    {0, 6, 0, 0, 6, 5},
    {0, 0, 0, 0, 6, 0},
    {0, 0, 6, 0, 0, 5},
    {0, 6, 0, 6, 6, 0},
    {0, 5, 6, 0, 0, 0}
    },
    {{0, 5, 0, 0, 0, 0},
    {0, 6, 6, 0, 0, 5},
    {5, 0, 6, 6, 0, 0},
    {0, 5, 0, 0, 0, 5},
    {0, 0, 0, 0, 0, 0},
    {6, 0, 0, 5, 0, 0}
    },
    {{0, 0, 5, 0, 0, 6},
    {0, 0, 0, 0, 5, 5},
    {0, 0, 0, 0, 0, 0},
    {6, 6, 0, 6, 0, 0},
    {0, 0, 6, 6, 0, 0},
    {0, 0, 5, 0, 6, 0}
    },
    {{0, 0, 0, 0, 0, 5},
    {5, 0, 0, 0, 0, 6},
    {0, 0, 6, 5, 0, 0},
    {0, 0, 0, 0, 0, 6},
    {0, 0, 5, 5, 0, 6},
    {0, 0, 5, 0, 0, 0}
    },
};*/