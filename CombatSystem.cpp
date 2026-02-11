#include "CombatSystem.h"
#include "DiceSystem.h"
#include "Player.h"
#include "Unit.h"

#include <queue>
#include <unordered_map>
#include <iostream>
#include <algorithm>

// --------------------------------------------------
// Agent HP (PDF)
// --------------------------------------------------
int CombatSystem::getAgentHP(AgentType type)
{
    switch (type) {
    case AgentType::Scout:     return 5;
    case AgentType::Sniper:    return 4;
    case AgentType::Seargeant: return 3;
    default:                  return 0;
    }
}

// --------------------------------------------------
// Dice count (PDF)
// --------------------------------------------------
int CombatSystem::getDiceCount(AgentType type)
{
    switch (type) {
    case AgentType::Scout:     return 1;
    case AgentType::Sniper:    return 3;
    case AgentType::Seargeant: return 1;
    default:                  return 0;
    }
}

// --------------------------------------------------
// TEMP: pick any placed defender unit
// --------------------------------------------------
Unit* CombatSystem::findTargetUnit(Player* defender)
{
    if (!defender) return nullptr;

    for (const auto& piece : defender->getPieces()) {
        if (piece && piece->getPosition())
            return piece.get();
    }
    return nullptr;
}

// --------------------------------------------------
// BFS shortest path
// --------------------------------------------------
std::vector<Cell*> CombatSystem::shortestPath(Cell* start, Cell* goal)
{
    std::vector<Cell*> path;
    if (!start || !goal) return path;

    std::queue<Cell*> q;
    std::unordered_map<Cell*, Cell*> parent;

    q.push(start);
    parent[start] = nullptr;

    while (!q.empty()) {
        Cell* current = q.front();
        q.pop();

        if (current == goal)
            break;

        for (Cell* neighbor : current->neighbors) {
            if (!neighbor) continue;
            if (parent.count(neighbor)) continue;

            parent[neighbor] = current;
            q.push(neighbor);
        }
    }

    if (!parent.count(goal))
        return {};

    for (Cell* c = goal; c != nullptr; c = parent[c])
        path.push_back(c);

    std::reverse(path.begin(), path.end());
    return path;
}

// --------------------------------------------------
// Sum ground shields (exclude endpoints)
// --------------------------------------------------
int CombatSystem::sumGroundShields(const std::vector<Cell*>& path)
{
    if (path.size() < 3)
        return 0;

    int sum = 0;
    for (size_t i = 1; i + 1 < path.size(); ++i)
        sum += path[i]->type;

    return sum;
}

// --------------------------------------------------
// FULL ATTACK RESOLUTION (PDF)
// --------------------------------------------------
bool CombatSystem::performAttack(
    AgentType agentType,
    Player* attacker,
    Player* defender
    ) {
    if (!attacker || !defender)
        return false;

    Unit* attackerUnit = attacker->getAgentPiece(agentType);
    Unit* targetUnit   = findTargetUnit(defender);

    if (!attackerUnit || !attackerUnit->getPosition() || !targetUnit) {
        std::cout << "Attacker or target unit missing/unplaced\n";
        return false;
    }

    // Shortest path
    auto path = shortestPath(
        attackerUnit->getPosition(),
        targetUnit->getPosition()
        );

    if (path.empty()) {
        std::cout << "No path to target\n";
        return false;
    }

    std::cout << "Shortest path: ";
    for (Cell* c : path)
        std::cout << c->tileId.toStdString() << " ";
    std::cout << std::endl;

    // Ground shields
    int shieldSum = sumGroundShields(path);
    std::cout << "Ground shield sum: " << shieldSum << std::endl;

    // Target HP
    int targetHP = getAgentHP(
        targetUnit->getType() == Unit::Type::SCOUT
            ? AgentType::Scout
            : targetUnit->getType() == Unit::Type::SNIPER
                  ? AgentType::Sniper
                  : AgentType::Seargeant
        );

    std::cout << "Target HP: " << targetHP << std::endl;

    // Attack criterion (clamp to 10)
    int attackCriterion = shieldSum + targetHP;
    if (attackCriterion > 10)
        attackCriterion = 10;

    std::cout << "Attack criterion: " << attackCriterion << std::endl;

    // Dice roll
    auto rolls = DiceSystem::rollD10(getDiceCount(agentType));
    std::cout << "Dice rolled: ";
    for (int r : rolls) std::cout << r << " ";
    std::cout << std::endl;

    // Success if ANY die > criterion
    for (int r : rolls) {
        if (r > attackCriterion) {
            defender->removeOneCardOfType(agentType);
            std::cout << "Attack successful!" << std::endl;
            return true;
        }
    }

    std::cout << "Attack failed." << std::endl;
    return false;
}
