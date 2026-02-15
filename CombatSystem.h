#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include "Cell.h"
#include <vector>

class Player;
class Unit;

class CombatSystem {
public:
    static bool performAttack(
        AgentType agentType,
        Player* attacker,
        Player* defender,
        Cell* targetCell
        );

private:
    static int getAgentHP(AgentType type);
    static int getDiceCount(AgentType type);

    static std::vector<Cell*> shortestPath(
        Cell* start,
        Cell* goal
        );

    static int sumGroundShields(
        const std::vector<Cell*>& path
        );
};

#endif
