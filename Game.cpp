#include "Game.h"

Game::Game()
{
}

void Game::startGame()
{
    started = true;
    currentPlayerIndex = 0;
}

void Game::nextTurn()
{
    if (players.empty())
        return;

    currentPlayerIndex = (currentPlayerIndex + 1) % players.size();
}

bool Game::validateMove()
{
    // Phase 2 – rules will live here later
    return true;
}
