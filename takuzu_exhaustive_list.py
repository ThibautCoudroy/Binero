"""Takuzu solver & generator (6x6 only)"""

LINE, COLUMN = 0, 1

line_possibilities = [
    [1, 1, 2, 1, 2, 2],
    [1, 1, 2, 2, 1, 2],
    [1, 2, 1, 1, 2, 2],
    [1, 2, 1, 2, 1, 2],
    [1, 2, 1, 2, 2, 1],
    [1, 2, 2, 1, 1, 2],
    [1, 2, 2, 1, 2, 1],
    [2, 2, 1, 2, 1, 1],
    [2, 2, 1, 1, 2, 1],
    [2, 1, 2, 2, 1, 1],
    [2, 1, 2, 1, 2, 1],
    [2, 1, 2, 1, 1, 2],
    [2, 1, 1, 2, 2, 1],
    [2, 1, 1, 2, 1, 2],
]

def solve_step(grid):
    """Returns boolean about if something was done (False = stuck)"""
    modif = False
    # - Complete line and column
    for mode in [LINE, COLUMN]:
        for i in range(6):
            # Get line or column (avoids copy paste for columns checks)
            list = [grid[i][k] for k in range(6)] if mode == LINE else [grid[k][i] for k in range(6)]
            # - If complete : nothing to be done
            if 0 in list:
                # - Count colors
                cnt = [0, 0, 0]
                for j in range(6): cnt[list[j]] += 1
                # - If one color is completed, finish the other
                for col in [1, 2]:
                    if cnt[col] == 3:
                        for k in range(6):
                            if list[k] == 0: list[k] = 3-col
                        modif = True
                        break
                # - Complete if 2 of same color next to eachother
                for j in range(5):
                    if list[j] == list[j+1] > 0:
                        if j+2 <= 5 and not list[j+2]:
                            list[j+2] = 3-list[j]
                            modif = True
                        if j-1 >= 0 and not list[j-1]:
                            list[j-1] = 3-list[j]
                            modif = True
                # - Complete if 2 of same color separated by blank
                for j in range(4):
                    if list[j] == list[j+2] > 0 and not list[j+1]:
                        list[j+1] = 3-list[j]
                        modif = True
                # - Complete if 2 '0' missing and another line has 4 in common
                cnt_0 = 0
                for j in range(6):
                    if not list[j] : cnt_0 += 1
                if cnt_0 == 2:
                    for j in range(6):
                        if i != j:
                            # Get the other list
                            list2 = [grid[j][k] for k in range(6)] if mode == LINE else [grid[k][j] for k in range(6)]
                            if not 0 in list2:
                                common = [0, 0, 0, 0, 0, 0]
                                for k in range(6):
                                    if list[k] == list2[k] > 0:
                                        common[k] = 1
                                if sum(common) == 4:
                                    for k in range(6):
                                        if not list[k]: list[k] = 3 - list2[k]
                                    modif = True
                                    break

            # - Done : writing in the original grid
            if mode == LINE:
                for j in range(6):
                    grid[i][j] = list[j]
            else:
                for j in range(6):
                    grid[j][i] = list[j]

    return modif

def has_error(grid):
        # - Test columns
        for i in range(6):
            cnt = [0, 0, 0]
            three_aligned = False
            for j in range(6):
                cnt[grid[j][i]] += 1
                if j<4 and grid[j][i] == grid[j+1][i] == grid[j+2][i] > 0:
                    three_aligned = True
            if cnt[1] > 3 or cnt[2] > 3 or three_aligned:
                return True # ERROR : >3 in a line or 3 aligned

        # - Test lines
        for i in range(6):
            cnt = [0, 0, 0]
            three_aligned = False
            for j in range(6):
                cnt[grid[i][j]] += 1
                if j<4 and grid[i][j] == grid[i][j+1] == grid[i][j+2] > 0:
                    three_aligned = True
            if cnt[1] > 3 or cnt[2] > 3 or three_aligned:
                return True # ERROR : >3 in a line or 3 aligned

        # - Test duplicates
        for i in range(6):
            col2 = [grid[k][i] for k in range(6)]
            for j in range(i):
                # - Lines
                if not 0 in grid[i] and grid[i] == grid[j]:
                    return True
                # - Columns
                col1 = [grid[k][j] for k in range(6)]
                if not 0 in col2 and col1 == col2:
                    return True

        return False

def grid_hash(grid):
    indexes = []
    for line in grid:
        indexes.append(line_possibilities.index(line))
    return tuple(indexes)

def mirror_grid(grid):
    return [grid[5-i] for i in range(6)]

def rotate_grid(grid):
    return [[grid[5-j][i] for j in range(6)] for i in range(6)]


def grid_hashes(grid):
    # returns hash and corresponding hashes with transformations
    # up to 16 possibilities (only one mirror because it's same as other mirror + 180°)
    hashes_list = []
    for color_swap in range(2):
        for mirror in range(4):
            grid = mirror_grid(grid)
            for rotation in range(4):
                grid = rotate_grid(grid)
                indexes = []
                for line in grid:
                    indexes.append((line_possibilities.index(line)+7*color_swap)%14)
                hashes_list.append(tuple(indexes))
    return hashes_list


"""Generate init state"""
total = 0
boards = set()  # all boards (4140)
boards_modulo_transformations = []  # unique boards

for i in range(14):
    for j in range(14):
        for k in range(14):
            for l in range(14):
                for m in range(14):
                    first_five_lines = [i,j,k,l,m]
                    grid = [line_possibilities[line] for line in first_five_lines] + [[0] * 6]
                    solve_step(grid) # generate last line
                    if not has_error(grid):
                        # for g in grid:
                        #     print(g)
                        h = grid_hash(grid)
                        if h not in boards:
                            boards_modulo_transformations.append(h)
                            for hashes in grid_hashes(grid):
                                boards.add(hashes)

                    total+=1

# ALL POSSIBLE BOARDS (4140)
boards_as_list = list(boards)
print(len(boards_as_list))
print("{")
for i in range(len(boards_as_list)//2):
    temp = boards_as_list[2*i][:5] + boards_as_list[2*i + 1][:5]
    temp2 = []
    for j in range(5):
        temp2.append((temp[2*j] << 4) + temp[2*j + 1])
    print("    ",str(temp2).replace('[','{').replace(']','}'),",", sep="")
print("};")

# # UNIQUE BOARD MODULO TRANSFORMATIONS (321)
# print(f"{len(boards_modulo_transformations)} unique boards found (modulo rotations, colors swap and mirroring)")
# print(boards_modulo_transformations)
# print("{")
# for i in range(len(boards_modulo_transformations)//2):
#     temp = boards_modulo_transformations[2*i][:5] + boards_modulo_transformations[2*i + 1][:5]
#     temp2 = []
#     for j in range(5):
#         temp2.append((temp[2*j] << 4) + temp[2*j + 1])
#     print("    ",str(temp2).replace('[','{').replace(']','}'),",", sep="")
# print("};")

