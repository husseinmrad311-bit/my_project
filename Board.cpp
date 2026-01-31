#include "Board.h"

void Board::buildNeighbors()
{
    // Clear any existing neighbors (safety if reloaded)
    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            grid[r][c].neighbors.clear();
        }
    }

    // 4-directional adjacency (up, down, left, right)
    const int dr[4] = { -1, 1, 0, 0 };
    const int dc[4] = { 0, 0, -1, 1 };

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {

            Cell& cell = grid[r][c];

            // Skip spacing / invalid cells
            if (cell.type < 0)
                continue;

            for (int i = 0; i < 4; ++i) {
                int nr = r + dr[i];
                int nc = c + dc[i];

                if (nr < 0 || nr >= rows || nc < 0 || nc >= cols)
                    continue;

                Cell& neighbor = grid[nr][nc];

                // Skip spacing neighbors
                if (neighbor.type < 0)
                    continue;

                cell.neighbors.push_back(&neighbor);
            }
        }
    }
}
