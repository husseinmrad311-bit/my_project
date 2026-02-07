// Player.h
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

    // Core properties
    int id = -1;
    std::string name;

    // Pieces (3 pieces as per PDF: Scout, Sniper, Sergeant)
    std::vector<std::unique_ptr<Unit>> pieces;

    // Cards (10 cards as per PDF: 4 Scout, 3 Sniper, 3 Sergeant)
    std::vector<Card*> deck;      // Full deck of 10 cards
    std::vector<Card*> hand;      // Cards drawn but not used

    // Simple getters
    int getId() const;
    const std::string& getName() const;
    const std::vector<std::unique_ptr<Unit>>& getPieces() const;

    // Initialize cards and pieces as per PDF
    void initializeCards();     // Creates 10 cards (4S, 3Ni, 3Se)
    void initializePieces();    // Creates 3 pieces (Scout, Sniper, Sergeant)

    // Card operations
    void shuffleDeck();
    Card* drawCard();           // Draw top card from deck
    void returnCardToBottom(Card* card); // Card goes to bottom after use

    // Piece operations
    Unit* getPiece(AgentType type) const;

    // Display
    void displayStatus() const;\

    Card* drawTopCard();                     // Draw from top of deck
    bool hasCards() const;                   // Check if deck has cards
    int getDeckSize() const;                 // Get remaining cards in deck

    // Piece access
    Unit* getAgentPiece(AgentType agentType) const;

    // Action validation helpers
    bool canPerformAction(AgentType agentType, const std::string& actionType) const;
    bool removeOneCardOfType(AgentType type);
private:
    Card* createCard(AgentType type);
    std::unique_ptr<Unit> createPiece(AgentType type);
};

#endif
