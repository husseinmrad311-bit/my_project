#ifndef BOARD_H
#define BOARD_H

#include "Cell.h"
#include <vector>
#include <memory>

// ================================
// Board ( with proper copy/move semantics)
// ================================

class Board {
public:
    int rows = 0;
    int cols = 0;

    // grid[r][c]
    std::vector<std::vector<Cell>> grid;
    void debugNeighbors(const QString& tileId) ;
    Board() = default;
    void ensureBidirectional() ;
    // copy constructor - disabled to prevent pointer issues
    Board(const Board&) = delete;

    // copy assignment - disabled
    Board& operator=(const Board&) = delete;

    // move constructor
    Board(Board&& other) noexcept
        : rows(other.rows)
        , cols(other.cols)
        , grid(std::move(other.grid))
    {
        //after moving, rebuild all neighbor pointers
        rebuildNeighborPointers();
    }

    // MOVE assignment
    Board& operator=(Board&& other) noexcept {
        if (this != &other) {
            rows = other.rows;
            cols = other.cols;
            grid = std::move(other.grid);
            rebuildNeighborPointers();
        }
        return *this;
    }

    void buildNeighbors();

    //new method to rebuild neighbor pointers after move/copy
    void rebuildNeighborPointers();

    //helper to validate board integrity
    bool validate() const;
};

#endif // BOARD_H
