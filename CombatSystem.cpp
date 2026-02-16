#include "CombatSystem.h"
#include "DiceSystem.h"
#include "Player.h"
#include "Unit.h"

#include <queue>
#include <unordered_map>
#include <iostream>
#include <algorithm>

// --------------------------------------------------
// Agent HP
// --------------------------------------------------
int CombatSystem::getAgentHP(AgentType type)
{
    switch (type) {
    case AgentType::Scout:     return 5;
    case AgentType::Sniper:    return 4;
    case AgentType::Seargeant: return 3;
    default:                   return 0;
    }
}

// --------------------------------------------------
// Dice count
// --------------------------------------------------
int CombatSystem::getDiceCount(AgentType type)
{
    switch (type) {
    case AgentType::Scout:     return 1;
    case AgentType::Sniper:    return 3;
    case AgentType::Seargeant: return 1;
    default:                   return 0;
    }
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
// FULL ATTACK
// --------------------------------------------------
bool CombatSystem::performAttack(
    AgentType agentType,
    Player* attacker,
    Player* defender,
    Cell* targetCell
    )
{

    if (!attacker || !defender || !targetCell)
        return false;

    Unit* attackerUnit = attacker->getAgentPiece(agentType);
    if (!attackerUnit || !attackerUnit->getPosition())
        return false;

    // Must contain a target
    if (targetCell->agent == AgentType::None)
        return false;

    // Safety: prevent self-attack
    Side attackerSide =
        (attacker->getId() == 1 ? Side::A : Side::B);

    if (targetCell->agentSide == attackerSide)
        return false;

    AgentType targetType = targetCell->agent;

    // Compute shortest path
    auto path = shortestPath(
        attackerUnit->getPosition(),
        targetCell
        );

    if (path.empty()) {
        std::cout << "No path to target\n";
        return false;
    }

    std::cout << "Shortest path: ";
    for (Cell* c : path)
        std::cout << c->tileId.toStdString() << " ";
    std::cout << std::endl;

    int shieldSum = sumGroundShields(path);
    std::cout << "Ground shield sum: " << shieldSum << std::endl;

    int targetHP = getAgentHP(targetType);
    std::cout << "Target HP: " << targetHP << std::endl;

    int attackCriterion = shieldSum + targetHP;
    if (attackCriterion > 10)
        attackCriterion = 10;

    std::cout << "Attack criterion: " << attackCriterion << std::endl;

    auto rolls = DiceSystem::rollD10(getDiceCount(agentType));

    std::cout << "Dice rolled: ";
    for (int r : rolls)
        std::cout << r << " ";
    std::cout << std::endl;

    for (int r : rolls)
    {
        if (r > attackCriterion)
        {
            std::cout << "DEBUG: Removing card of type: "
                      << (targetType == AgentType::Scout ? "Scout" :
                              targetType == AgentType::Sniper ? "Sniper" : "Sergeant")
                      << " for defender\n";
            defender->removeOneCardOfType(targetType);
            return true;
        }
    }

    std::cout << "Attack failed.\n";
    return false;
}
