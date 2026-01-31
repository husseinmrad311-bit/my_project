#ifndef CELL_H
#define CELL_H

#include <QString>
#include <vector>

// Forward declarations (no includes, no coupling)
class Player;
class Agent;
class Cell;

class Cell {
public:
    QString tileId;  //"A01"
    int type = 0;    //0,1,2...

    // ===== Phase 2 additions (Day 1) =====

    bool shield = false;

    Agent* agent = nullptr;

    Player* markedBy = nullptr;
    Player* controlledBy = nullptr;

    std::vector<Cell*> neighbors;
};

#endif
