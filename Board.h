#ifndef BOARD_H
#define BOARD_H

#include "Cell.h"
#include <vector>

// ================================
// Board (pure data container)
// ================================

class Board {
public:
    int rows = 0;
    int cols = 0;

    // grid[r][c]
    std::vector<std::vector<Cell>> grid;

    void buildNeighbors();
};

#endif // BOARD_H
