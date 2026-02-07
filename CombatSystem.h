#ifndef COMBATSYSTEM_H
#define COMBATSYSTEM_H

#include "Cell.h"

class Player;

class CombatSystem {
public:
    // Performs attack based on agent type
    // Returns true if attack succeeds
    static bool performAttack(
        AgentType agentType,
        Player* attacker,
        Player* defender
        );
};

#endif
