#ifndef GAME_H
#define GAME_H

#include "Board.h"
#include "Player.h"
#include <vector>
#include <memory>
#include <string>

class Game {
public:
    Game(const std::string& name1,const std::string& name2);
    bool loadMap(const std::string& path);
    // Core state
    Board board;
    std::vector<std::unique_ptr<Player>> players;

    // Turn system state
    int currentPlayerIndex = 0;
    bool gameStarted = false;
    bool gameOver = false;
    int turnCount = 0;
    Card* currentCard = nullptr;

    // Lifecycle
    void startGame();
    void nextTurn();
    void endTurn();
    bool isGameOver() const;

    // Card actions
    bool drawCard();
    bool performAction(const std::string& actionType,const std::string& target = "");
    bool returnCardToBottom();

    // Actions
    bool movePiece(AgentType agentType, const std::string& targetCoord);
    bool attackPiece(AgentType agentType, const std::string& targetCoord);
    bool scoutTile(const std::string& targetCoord);
    bool controlTile(const std::string& targetCoord);
    bool releaseTile(const std::string& targetCoord);

    // Game state
    Player* getCurrentPlayer() const;
    Player* getOpponent() const;
    Card* getCurrentCard() const { return currentCard; }
    const Board& getBoard() const;

    void displayGameState() const;
    void displayTurnInfo() const;

private:

    void initializePlayers();
    void initializeBoard();

    bool isActionAllowed(AgentType agentType,
                         const std::string& actionType);

    // 🔑 MUST MATCH Game.cpp EXACTLY
    void checkWinConditions();
    int countControlledHouses(Player* player) const;
};

#endif
