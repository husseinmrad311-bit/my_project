#include "DiceSystem.h"
#include <random>

std::vector<int> DiceSystem::rollD10(int count)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(1, 10);

    std::vector<int> results;
    for (int i = 0; i < count; ++i)
        results.push_back(dist(gen));

    return results;
}
