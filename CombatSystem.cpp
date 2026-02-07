#include "CombatSystem.h"
#include "DiceSystem.h"
#include "Player.h"
#include <iostream>

bool CombatSystem::performAttack(
    AgentType agentType,
    Player* attacker,
    Player* defender
    ) {
    if (!attacker || !defender) return false;

    int diceCount = 0;

    switch (agentType) {
    case AgentType::Scout:
        diceCount = 1;
        break;
    case AgentType::Sniper:
        diceCount = 3;
        break;
    case AgentType::Seargeant:
        diceCount = 1;
        break;
    default:
        return false;
    }

    // Placeholder attack criterion (will be replaced later)
    const int attackThreshold = 5;

    auto rolls = DiceSystem::rollD10(diceCount);

    std::cout << "Dice rolled: ";
    for (int r : rolls) std::cout << r << " ";
    std::cout << std::endl;

    for (int r : rolls) {
        if (r > attackThreshold) {
            defender->removeOneCardOfType(agentType);
            std::cout << "Attack successful!" << std::endl;
            return true;
        }
    }

    std::cout << "Attack failed." << std::endl;
    return false;
}
