#include <stdbool.h>
#include <stdint.h>
#include <rand.h>
#include <gb/gb.h>
#include <gb/cgb.h>
#include "puzzles_gen.h"
#include "data/puzzles_data.h"

#define NB_TILES_REMOVED 4
#define NB_UNDO_MAX 13

bool hard_mode = false;
bool diff_value(void) {return hard_mode;}
void switch_diff(void) {hard_mode = !hard_mode;}

uint8_t board_temp[6][6];
uint8_t board_statement[6][6];
uint8_t removed_history[NB_TILES_REMOVED][3];
bool solve_board(bool modif);

const uint8_t lines_poss[][6] = {
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


void get_valid_sol(void) {
    uint16_t r_val = randw();
    // set_bkg_tiles(30,31,2,1,&r_val);
    uint16_t board_id = r_val & 0x01FF;     // 2048 pairs of unique boards
    uint8_t rand_val = (r_val >> 15) & 1;   // indicates first or second board of the pair
    // 4096 puzzles, the 16 bit comes from the random removing of tiles, which depends on the 16 bit random
    
    // Decode puzzle data
    for(uint8_t i = 0, j = rand_val * 5; i < 5; i++, j++) {
        uint8_t temp_line = puzzle_data[board_id][j/2];
        if(j&1) temp_line &= 0x0F;
        else    temp_line >>= 4;
        for(uint8_t j=0; j<6; j++) {
            board_statement[i][j] = lines_poss[temp_line][j];     // Copy the line into temp board
            board_statement[5][j] = 0;   // Last line is empty because it's gonna be deduced
        }
    }

    // Legacy code, useless now since all boards are stored in the cartridge
    // // 50% chance of colors swap
    // if((r_val >> 14)&1) {
    //     // Shift 7 from each line possibility
    //     for(uint8_t i=0; i<6; i++) {
    //         temp_lines[i] = (temp_lines[i] + 7)%14;
    //     }
    // }

    // // 50% chance of copying mirror
    // rand_val = (r_val >> 13)&1;
    // for(uint8_t l=0; l<5; l++) {
    //     for(uint8_t j=0; j<6; j++) {
    //         temp_board[l][j] = lines_poss[temp_lines[l]][(rand_val ? 5-j : j)];     // Copy the line into temp board
    //         temp_board[5][j] = 0;   // Last line is empty because it's gonna be deduced
    //     }
    // }

    // // 25% chance for every rotation
    // rand_val = (r_val >> 11)&3;
    // uint8_t t1, t2;
    // for(uint8_t k=0; k<6; k++) {
    //     for(uint8_t m=0; m<6; m++) {
    //         if(rand_val == 0) {
    //             t1 = k;
    //             t2 = m;
    //         } else if(rand_val == 1) {
    //             t1 = m;
    //             t2 = 5-k;
    //         } else if(rand_val == 2) {
    //             t1 = 5-k;
    //             t2 = 5-m;
    //         } else {
    //             t1 = 5-m;
    //             t2 = k;
    //         }

    //         board_statement[k][m] = temp_board[t1][t2];
    //     }
    // }

    // Solve last 6 tiles
    solve_board(true);
}

// Removes "tiles_removed_step" tiles of the board
void remove_tiles(uint8_t tiles_removed_step) {
    uint8_t nb_removed = 0;
    while(nb_removed < tiles_removed_step) {
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
void undo_remove_tiles(uint8_t tiles_removed_step) {
    for(uint8_t i=0; i < tiles_removed_step; i++) {
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
                    if (hard_mode) {
                        uint8_t first_col = list1[0]; // 0 means no color
                        uint8_t last_col = list1[5]; // 0 means no color
                        // Same color at first and last position means opposite color next to them
                        if (first_col && first_col == last_col) {
                            if (!list1[1] || !list1[4]) {
                                uint8_t col = 3-first_col;
                                list1[1] = col;
                                list1[4] = col;
                                modif = true;
                            }
                        }
                        // Same color at two first position then other means other color at last position
                        // Or same color at first and 5th position means other color at last position
                        else if (first_col && !last_col) {
                            if ((first_col==list1[4]) || (first_col==list1[1] && first_col==3-list1[2])) {
                                list1[5]=3-first_col;
                                modif = true;
                            }
                        }
                        // Symmetric as above
                        else if (last_col && !first_col) {
                            if ((last_col==list1[1]) || (last_col==list1[4] && last_col==3-list1[3])) {
                                list1[0]=3-last_col;
                                modif = true;
                            }
                        }
                    }

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

                    // Fill based on completed lines
                    uint8_t cnt_0 = 6;
                    for(incr=0; incr<6; incr++) {
                        if(*(board_temp[0] + (axis ? 6*incr + i : incr + 6*i))) cnt_0--;
                    }
                    // - If two '0' left and another line has 4 in common
                    // - If three '0' left and another line has 3 in common
                    if(cnt_0 == 2 || cnt_0 == 3) {
                        for(uint8_t j=i+1; j<6; j++) {

                            hasZero = false;

                            // List2 copies the other line/column
                            for(incr=0; incr<6; incr++) {
                                val = *(board_temp[0] + (axis ? 6*incr + j : incr + 6*j));
                                if(val == 0) { // If not complete, test next line 
                                    hasZero = true;
                                    break;
                                }
                                list2[incr] = val;
                            }
                            if(!hasZero) {
                                // Check if common, else count the other cells
                                uint8_t sum = 0;
                                uint8_t count_ones=0;
                                for(incr=0; incr<6; incr++) {
                                    if(list1[incr] == list2[incr]) {
                                        sum++;
                                        if(list1[incr]==1) count_ones++;
                                    }
                                }

                                if(cnt_0 == 2 && sum == 4) {
                                    for(incr=0; incr<6; incr++) {
                                        if(!list1[incr]) list1[incr] = 3 - list2[incr];
                                    }
                                    modif = true;
                                    break;
                                }
                                else if(hard_mode && cnt_0 == 3 && sum == 3) {
                                    // 3 cells in common implies 2 of one color, 1 of the other (e.g. 12xxx2)
                                    // and an empty cell can be filled with "other color" as well
                                    // the one where the other line does not have "other color" (e.g. 121212 => 12x1x2)
                                    // optim done: trick where the counter of ones also gives ID of cell to fill
                                    for(incr=0; incr<6; incr++) {
                                        if(!list1[incr] && list2[incr]!=count_ones) {
                                            list1[incr] = count_ones;
                                            break;
                                        }
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

uint8_t progress;
void update_loading_bar(void) {
    if (progress<10) progress++; // 10 sprites limit
    set_sprite_prop(progress + 29, 6);
}

void generate_board(uint8_t board[6][6]) {
    cpu_fast();
    // STEP 1 : BOARD_STATEMENT GETS A VALID SOLUTION
    get_valid_sol();
    progress = 0;

    // STEP 2 : REMOVE TILES FROM STATEMENT
    uint8_t tiles_removed_step = NB_TILES_REMOVED;
    uint8_t nb_undo = 0; // Number of times a necessary tile was removed
    uint8_t nb_removed = 0; // Make sure at least four tiles are removed

    while(nb_undo < NB_UNDO_MAX || (nb_removed<4)) {
        remove_tiles(tiles_removed_step);

        // DEBUG VISUALS
        // COPY CURRENT STATE
        // for(uint8_t i=0; i<12; i+=2) {
        //     for(uint8_t j=0; j<12; j+=2) {
        //         uint8_t cell = board_statement[j>>1][i>>1];
        //         uint8_t col = cell & 3;
        //         uint8_t tmp[] = {col, col, col, col};
        //         VBK_REG = 1;
        //         set_bkg_tiles(4 + i, 4 + j, 2, 2, tmp);
        //         VBK_REG = 0;
        //         const unsigned char normal_cell[] = {52,53,54,55};
        //         set_bkg_based_tiles(4 + i, 4 + j, 2, 2, normal_cell, cell & 4);
        //     }
        // }
        // if (!board_statement[0][1] && !board_statement[1][0] && tiles_removed_step==1 && nb_undo>9) {
        //     solve_board(true);
        //     waitpad(0xFF);
        //     waitpadup();
        // }

        // If not solvable anymore, we undo
        if(!solve_board(false)) {
            undo_remove_tiles(tiles_removed_step);
            nb_undo++;
            update_loading_bar();
            if(tiles_removed_step>1) tiles_removed_step--;  // Removing tiles more slowly
        } else {
            nb_removed += tiles_removed_step;
        }
    }

    // STEP 3 : BOARD COPIES BOARD_STATEMENT
    for(uint8_t k=0; k<36; k++) {
        uint8_t t = *(board_statement[0] + k);
        *(board[0] + k) = t ? t+4 : 0;
    }
    
    wait_vbl_done();
    cpu_slow();
}
