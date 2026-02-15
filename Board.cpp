#include "Board.h"
#include <iostream>

void Board::buildNeighbors()
{
    // Clear all neighbors first
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            grid[r][c].neighbors.clear();
        }
    }

    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            Cell& cell = grid[r][c];
            if (cell.type < 0 || cell.tileId.isEmpty())
                continue;

            // LEFT
            if (c > 0 && grid[r][c-1].type >= 0) {
                cell.neighbors.push_back(&grid[r][c-1]);
                // Don't add reverse here - will be added when processing that cell
            }

            // RIGHT
            if (c < cols-1 && grid[r][c+1].type >= 0) {
                cell.neighbors.push_back(&grid[r][c+1]);
            }

            // Determine row type
            bool isBRow = cell.tileId.startsWith('B');
            bool isARow = cell.tileId.startsWith('A');

            // UP (to previous row)
            if (r > 0)
            {
                int targetCol = c;

                if (isBRow) {
                    // B row connects to A row above at same column? Or column-1?
                    // From your working connections: B01 should connect to A06 and A01?
                    targetCol = c;  // B01 at (1,0) should connect to A06 at (2,0) and A01 at (0,0)?
                } else if (isARow) {
                    // A row connects to B row above at column?
                    targetCol = c;  // A06 at (2,0) connects to B01 at (1,0) - this works
                }

                if (targetCol >= 0 && targetCol < cols) {
                    Cell& above = grid[r-1][targetCol];
                    if (above.type >= 0) {
                        cell.neighbors.push_back(&above);
                    }
                }
            }

            // DOWN (to next row)
            if (r < rows-1)
            {
                int targetCol = c;

                if (isBRow) {
                    // B row connects to A row below
                    targetCol = c;  // B01 at (1,0) should connect to A06 at (2,0)
                } else if (isARow) {
                    // A row connects to B row below
                    targetCol = c;  // A06 at (2,0) connects to B07? Wait, B07 is at (3,2) - wrong!
                }

                if (targetCol >= 0 && targetCol < cols) {
                    Cell& below = grid[r+1][targetCol];
                    if (below.type >= 0) {
                        cell.neighbors.push_back(&below);
                    }
                }
            }
        }
    }

    // DEBUG: Verify bidirectional connections
    std::cout << "\n=== VERIFYING BIDIRECTIONAL CONNECTIONS ===\n";
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell& cell = grid[r][c];
            if (cell.type < 0) continue;

            for (Cell* neighbor : cell.neighbors) {
                bool foundReverse = false;
                for (Cell* reverseNeighbor : neighbor->neighbors) {
                    if (reverseNeighbor == &cell) {
                        foundReverse = true;
                        break;
                    }
                }
                if (!foundReverse) {
                    std::cout << "WARNING: " << cell.tileId.toStdString()
                    << " -> " << neighbor->tileId.toStdString()
                    << " is NOT bidirectional!\n";
                }
            }
        }
    }
}

void Board::ensureBidirectional()
{
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell& cell = grid[r][c];
            if (cell.type < 0) continue;

            // For each neighbor, ensure the reverse connection exists
            std::vector<Cell*> newNeighbors;
            for (Cell* neighbor : cell.neighbors) {
                bool reverseExists = false;
                for (Cell* rev : neighbor->neighbors) {
                    if (rev == &cell) {
                        reverseExists = true;
                        break;
                    }
                }
                if (!reverseExists) {
                    neighbor->neighbors.push_back(&cell);
                    std::cout << "Added missing reverse: "
                              << neighbor->tileId.toStdString()
                              << " -> " << cell.tileId.toStdString() << "\n";
                }
            }
        }
    }
}

void Board::debugNeighbors(const QString& tileId)
{
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            Cell& cell = grid[r][c];
            if (cell.tileId == tileId)
            {
                std::cout << "Neighbors of " << tileId.toStdString() << ": ";
                for (Cell* neighbor : cell.neighbors)
                {
                    if (neighbor)
                        std::cout << neighbor->tileId.toStdString() << " ";
                }
                std::cout << std::endl;
                return;
            }
        }
    }
}

void Board::rebuildNeighborPointers()
{
    // This is called after a move operation to ensure all neighbor
    // pointers point to cells within this board instance

    // Simply rebuild all neighbors from scratch
    buildNeighbors();
}

bool Board::validate() const
{
    for (int r = 0; r < rows; ++r)
    {
        for (int c = 0; c < cols; ++c)
        {
            const Cell& cell = grid[r][c];

            // Check each neighbor pointer
            for (Cell* neighbor : cell.neighbors)
            {
                if (!neighbor) continue;

                // Verify neighbor points to a cell within this board's grid
                bool found = false;
                for (int rr = 0; rr < rows && !found; ++rr)
                {
                    for (int cc = 0; cc < cols && !found; ++cc)
                    {
                        if (neighbor == &grid[rr][cc])
                        {
                            found = true;
                        }
                    }
                }

                if (!found)
                {
                    std::cout << "Board validation failed: neighbor pointer points outside board!\n";
                    return false;
                }
            }
        }
    }
    return true;
}
