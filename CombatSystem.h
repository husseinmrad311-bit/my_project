#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include "Cell.h"
#include <vector>

class Player;
class Unit;

class CombatSystem {
public:
    // Performs attack based on agent type
    static bool performAttack(
        AgentType agentType,
        Player* attacker,
        Player* defender
        );

private:
    // Agent HP (PDF)
    static int getAgentHP(AgentType type);

    // Dice count per attacker (PDF)
    static int getDiceCount(AgentType type);

    // BFS shortest path
    static std::vector<Cell*> shortestPath(
        Cell* start,
        Cell* goal
        );

    // Sum ground shields (exclude endpoints)
    static int sumGroundShields(
        const std::vector<Cell*>& path
        );

    // TEMPORARY: pick any placed defender unit
    static Unit* findTargetUnit(Player* defender);
};

#endif // COMBATSYSTEM_H
