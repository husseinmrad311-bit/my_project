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

    //disable copy
    Game(const Game&) = delete;
    Game& operator=(const Game&) = delete;

    // enable move
    Game(Game&&) = default;
    Game& operator=(Game&&) = default;

    bool loadMap(const std::string& path);

    // core state
    Board board;
    std::vector<std::unique_ptr<Player>> players;

    // turn system state
    int currentPlayerIndex = 0;
    bool gameStarted = false;
    bool gameOver = false;
    int turnCount = 0;
    Card* currentCard = nullptr;

    // lifecycle
    void startGame();
    void nextTurn();
    void endTurn();
    bool isGameOver() const;

    // card actions
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

    //Load state file
    bool loadStateFile(const std::string& path);

private:
    void initializePlayers();
    void initializeBoard();

    void synchronizeUnitsWithBoard();

    bool isActionAllowed(AgentType agentType,
                         const std::string& actionType);

    void checkWinConditions();
    int countControlledHouses(Player* player) const;
};

#endif
