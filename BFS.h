#ifndef BFS_H
#define BFS_H

#include <vector>
#include <functional>

class Cell;

class BFS {
public:
    // generic BFS traversal
    // -start : starting cell
    // -canVisit : optional condition to allow visiting a cell
    // -onVisit : optional callback when a cell is visited
    static std::vector<Cell*> traverse(Cell* start,std::function<bool(Cell*)> canVisit = nullptr,std::function<void(Cell*)> onVisit = nullptr);
};

#endif
