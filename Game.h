#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <vector>
#include <memory>

class Game {
public:
    Game();

    // Core state
    Board board;
    std::vector<std::unique_ptr<Player>> players;

    // Turn system state
    int currentPlayerIndex = 0;
    bool gameStarted = false;
    int turnCount = 0;
    Card* currentCard = nullptr; // Card drawn this turn

    // Lifecycle
    void startGame();
    void nextTurn();
    void endTurn();
    bool isGameOver() const;

    // Card actions (as per PDF)
    bool drawCard();                     // Draw top card for current player
    bool performAction(const std::string& actionType, const std::string& target = "");
    bool returnCardToBottom();           // Move current card to bottom of deck

    // Specific actions (will be expanded later)
    bool movePiece(AgentType agentType, const std::string& targetCoord);
    bool attackPiece(AgentType agentType, const std::string& targetCoord);
    bool scoutTile(const std::string& targetCoord);
    bool controlTile(const std::string& targetCoord);
    bool releaseTile(const std::string& targetCoord);

    // Getters
    Player* getCurrentPlayer() const;
    Player* getOpponent() const;
    Player* getPlayer(int id) const;

    // Game state display
    void displayGameState() const;
    void displayTurnInfo() const;

private:
    void initializePlayers();
    void initializeBoard();
    bool isActionAllowed(AgentType agentType, const std::string& actionType);
};

#endif
