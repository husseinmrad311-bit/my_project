#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"

#include <vector>

class Game {
public:
    Game();

    // Core state
    Board board;
    std::vector<Player*> players;

    int currentPlayerIndex = 0;
    bool started = false;

    // Lifecycle
    void startGame();
    void nextTurn();

    // Validation hook (stub for Phase 2 rules)
    bool validateMove(); // intentionally empty for now
};

#endif
