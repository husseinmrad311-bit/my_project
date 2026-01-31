#ifndef PLAYER_H
#define PLAYER_H

#include <vector>

class Player {
public:
    int id = -1;

    // Placeholder for Phase 2 cards
    std::vector<int> deck;

    Player() = default;
    explicit Player(int playerId);
};

#endif
