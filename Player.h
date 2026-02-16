#ifndef PLAYER_H
#define PLAYER_H

#include <vector>
#include <memory>
#include <string>
#include "Unit.h"
#include "Card.h"

class Player {
public:
    Player() = default;
    explicit Player(int playerId);
    Player(int playerId, const std::string& playerName);   // constructor

    // core properties
    int id = -1;
    std::string name;

    // Pieces (Scout/Sniper/Sergeant)
    std::vector<std::unique_ptr<Unit>> pieces;

    // Cards (4 Scout, 3 Sniper, 3 Sergeant)
    std::vector<Card*> deck;
    std::vector<Card*> hand;

    //getters
    int getId() const;
    const std::string& getName() const;
    const std::vector<std::unique_ptr<Unit>>& getPieces() const;

    // Initialization
    void initializeCards();
    void initializePieces();
    //card operations
    void shuffleDeck();
    Card* drawCard();
    void returnCardToBottom(Card* card);

    // piece access
    Unit* getPiece(AgentType type) const;
    Unit* getAgentPiece(AgentType agentType) const;

    // display
    void displayStatus() const;

    // helpers
    Card* drawTopCard();
    bool hasCards() const;
    int getDeckSize() const;

    // action validation
    bool canPerformAction(AgentType agentType, const std::string& actionType) const;

    bool removeOneCardOfType(AgentType type);
    int countCardsOfType(AgentType type) const;

private:
    Card* createCard(AgentType type);
    std::unique_ptr<Unit> createPiece(AgentType type);
    void removeAgentPieceIfExists(AgentType type);
};

#endif
