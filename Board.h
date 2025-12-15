#ifndef BOARD_H
#define BOARD_H

#include "Cell.h"
#include <vector>
using namespace std ;
class Board {
public:
    int rows = 0;
    int cols = 0;
    vector<std::vector<Cell>> grid; // grid[r][c]
};

#endif
