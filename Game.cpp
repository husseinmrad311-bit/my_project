#include "Game.h"
#include "CombatSystem.h"
#include <iostream>

Game::Game() {
    initializePlayers();
    initializeBoard();
}

void Game::initializePlayers() {
    players.clear();
    players.push_back(std::make_unique<Player>(1));
    players.push_back(std::make_unique<Player>(2));
}

void Game::initializeBoard() {
    // Board initialized elsewhere (MapLoader / UI)
}

void Game::startGame() {
    if (gameStarted) return;

    gameStarted = true;
    turnCount = 1;
    currentPlayerIndex = 0;

    std::cout << "=== GAME STARTED ===" << std::endl;
    std::cout << "Player 1 vs Player 2" << std::endl;

    if (drawCard()) {
        displayTurnInfo();
    }

    displayGameState();
}

bool Game::drawCard() {
    Player* current = getCurrentPlayer();
    if (!current) return false;

    currentCard = current->drawTopCard();
    if (!currentCard) {
        std::cout << current->getName() << " has no cards to draw!" << std::endl;
        return false;
    }

    std::cout << current->getName() << " draws: ";
    currentCard->display();
    return true;
}

void Game::nextTurn() {
    if (!gameStarted) return;

    endTurn();
    currentPlayerIndex = (currentPlayerIndex + 1) % 2;
    turnCount++;

    std::cout << "\n=== TURN " << turnCount << " ===" << std::endl;
    std::cout << getCurrentPlayer()->getName() << "'s turn begins." << std::endl;

    if (drawCard()) {
        displayTurnInfo();
    }
}

void Game::endTurn() {
    if (currentCard) {
        returnCardToBottom();
    }
    currentCard = nullptr;
}

bool Game::returnCardToBottom() {
    if (!currentCard) return false;

    Player* current = getCurrentPlayer();
    current->returnCardToBottom(currentCard);
    return true;
}

bool Game::performAction(const std::string& actionType, const std::string& target) {
    if (!currentCard) {
        std::cout << "No card drawn for this turn!" << std::endl;
        return false;
    }

    AgentType agentType = currentCard->getType();

    if (!isActionAllowed(agentType, actionType)) {
        std::cout << "ERROR: Action not allowed for this agent\n";
        return false;
    }

    if (actionType == "move")
        return movePiece(agentType, target);
    if (actionType == "attack")
        return attackPiece(agentType, target);
    if (actionType == "scout")
        return scoutTile(target);
    if (actionType == "control")
        return controlTile(target);
    if (actionType == "release")
        return releaseTile(target);

    return false;
}

bool Game::isActionAllowed(AgentType agentType, const std::string& actionType) {
    switch (agentType) {
    case AgentType::Scout:
        return actionType == "move" || actionType == "scout" || actionType == "attack";
    case AgentType::Sniper:
        return actionType == "move" || actionType == "attack";
    case AgentType::Seargeant:
        return actionType == "move" || actionType == "attack"
               || actionType == "control" || actionType == "release";
    default:
        return false;
    }
}

// --------------------------------------------------
// MOVE
// --------------------------------------------------
bool Game::movePiece(AgentType agentType, const std::string& targetCoord) {
    Player* current = getCurrentPlayer();
    Unit* piece = current->getAgentPiece(agentType);
    if (!piece || !piece->getPosition()) return false;

    Cell* from = piece->getPosition();
    Cell* to = nullptr;

    for (Cell* n : from->neighbors) {
        if (n && n->tileId.toStdString() == targetCoord) {
            to = n;
            break;
        }
    }

    if (!to || to->isOccupied()) return false;

    // Sniper & Sergeant must move to marked tiles
    if (agentType != AgentType::Scout && !to->marked)
        return false;

    from->clearAgent();
    to->setAgent(agentType, current->getId() == 1 ? Side::A : Side::B);
    piece->setPosition(to);

    return true;
}

// --------------------------------------------------
// SCOUT
// --------------------------------------------------
bool Game::scoutTile(const std::string& targetCoord) {
    Player* current = getCurrentPlayer();
    Unit* scout = current->getAgentPiece(AgentType::Scout);
    if (!scout || !scout->getPosition()) return false;

    Cell* cell = scout->getPosition();

    if (!targetCoord.empty() &&
        cell->tileId.toStdString() != targetCoord)
        return false;

    if (cell->marked) return false;

    cell->marked = true;
    cell->markedBy = (current->getId() == 1 ? Side::A : Side::B);

    std::cout << current->getName()
              << " scouted tile "
              << cell->tileId.toStdString()
              << std::endl;

    return true;
}

// --------------------------------------------------
// ATTACK
// --------------------------------------------------
bool Game::attackPiece(AgentType agentType, const std::string&) {
    return CombatSystem::performAttack(
        agentType,
        getCurrentPlayer(),
        getOpponent()
        );
}

// --------------------------------------------------
// SERGEANT CONTROL
// --------------------------------------------------
bool Game::controlTile(const std::string& targetCoord)
{
    Player* current = getCurrentPlayer();
    Unit* sergeant = current->getAgentPiece(AgentType::Seargeant);
    if (!sergeant || !sergeant->getPosition())
        return false;

    Cell* cell = sergeant->getPosition();

    if (!targetCoord.empty() &&
        cell->tileId.toStdString() != targetCoord)
        return false;

    Side mySide = (current->getId() == 1 ? Side::A : Side::B);

    if (cell->controlledBy != Side::None)
        return false;

    if (cell->isOccupied() && cell->agentSide != mySide)
        return false;

    cell->controlledBy = mySide;

    std::cout << current->getName()
              << " controls tile "
              << cell->tileId.toStdString()
              << std::endl;

    return true;
}

// --------------------------------------------------
// SERGEANT RELEASE
// --------------------------------------------------
bool Game::releaseTile(const std::string& targetCoord)
{
    Player* current = getCurrentPlayer();
    Unit* sergeant = current->getAgentPiece(AgentType::Seargeant);
    if (!sergeant || !sergeant->getPosition())
        return false;

    Cell* cell = sergeant->getPosition();

    if (!targetCoord.empty() &&
        cell->tileId.toStdString() != targetCoord)
        return false;

    Side mySide = (current->getId() == 1 ? Side::A : Side::B);

    if (cell->controlledBy == Side::None ||
        cell->controlledBy == mySide)
        return false;

    if (cell->isOccupied() && cell->agentSide != mySide)
        return false;

    cell->controlledBy = Side::None;

    std::cout << current->getName()
              << " released tile "
              << cell->tileId.toStdString()
              << std::endl;

    return true;
}

// --------------------------------------------------

Player* Game::getCurrentPlayer() const {
    return players[currentPlayerIndex].get();
}

Player* Game::getOpponent() const {
    return players[(currentPlayerIndex + 1) % 2].get();
}

Player* Game::getPlayer(int id) const {
    for (const auto& p : players) {
        if (p->getId() == id)
            return p.get();
    }
    return nullptr;
}

bool Game::isGameOver() const {
    return false;
}

void Game::displayGameState() const {
    std::cout << "\n=== GAME STATE ===\n";
    for (const auto& p : players)
        p->displayStatus();
}

void Game::displayTurnInfo() const {
    std::cout << "\n--- CURRENT TURN ---\n";
    currentCard->display();
}
