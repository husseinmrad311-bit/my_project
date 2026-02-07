#include "Player.h"
#include"Unit.h"
#include <algorithm>
#include <random>
#include <iostream>

Player::Player(int playerId) {
    id = playerId;
    name = "Player " + std::to_string(playerId);

    // Initialize as per PDF
    initializeCards();
    initializePieces();
}

int Player::getId() const { return id; }
const std::string& Player::getName() const { return name; }
const std::vector<std::unique_ptr<Unit>>& Player::getPieces() const { return pieces; }

// ADD THIS METHOD - It was missing!
Card* Player::createCard(AgentType type) {
    return new Card(type);
}

std::unique_ptr<Unit> Player::createPiece(AgentType type) {
    // Convert AgentType to Unit::Type
    Unit::Type unitType;
    switch (type) {
    case AgentType::Scout: unitType = Unit::Type::SCOUT; break;
    case AgentType::Sniper: unitType = Unit::Type::SNIPER; break;
    case AgentType::Seargeant: unitType = Unit::Type::SERGEANT; break;
    default: unitType = Unit::Type::SCOUT;
    }

    // Create piece with ID based on type
    int pieceId = (id * 10) + static_cast<int>(type);
    return std::make_unique<Unit>(pieceId, id, unitType);
}

void Player::initializeCards() {
    deck.clear();

    // Create 4 Scout cards (as per PDF)
    for (int i = 0; i < 4; i++) {
        deck.push_back(createCard(AgentType::Scout));
    }

    // Create 3 Sniper cards (as per PDF)
    for (int i = 0; i < 3; i++) {
        deck.push_back(createCard(AgentType::Sniper));
    }

    // Create 3 Sergeant cards (as per PDF)
    for (int i = 0; i < 3; i++) {
        deck.push_back(createCard(AgentType::Seargeant));
    }

    shuffleDeck();  // This calls shuffleDeck() which we need to define
}

// ADD THIS METHOD - It was missing!
void Player::shuffleDeck() {
    if (deck.empty()) return;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

void Player::initializePieces() {
    pieces.clear();

    // Create 3 pieces as per PDF: Scout, Sniper, Sergeant
    pieces.push_back(createPiece(AgentType::Scout));
    pieces.push_back(createPiece(AgentType::Sniper));
    pieces.push_back(createPiece(AgentType::Seargeant));
}

Card* Player::drawCard() {
    if (deck.empty()) {
        return nullptr;
    }

    Card* card = deck.front();
    deck.erase(deck.begin());
    hand.push_back(card);

    return card;
}

void Player::returnCardToBottom(Card* card) {
    // Remove from hand
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end()) {
        hand.erase(it);
    }

    // Add to bottom of deck (as per PDF: card goes to bottom after use)
    deck.push_back(card);
}

Unit* Player::getPiece(AgentType type) const {
    for (const auto& piece : pieces) {
        if ((type == AgentType::Scout && piece->getType() == Unit::Type::SCOUT) ||
            (type == AgentType::Sniper && piece->getType() == Unit::Type::SNIPER) ||
            (type == AgentType::Seargeant && piece->getType() == Unit::Type::SERGEANT)) {
            return piece.get();
        }
    }
    return nullptr;
}

void Player::displayStatus() const {
    std::cout << "\n=== " << name << " ===" << std::endl;

    // Display pieces
    std::cout << "Pieces (3 total):" << std::endl;
    for (const auto& piece : pieces) {
        std::cout << "  ";
        piece->display();
    }

    // Display deck composition
    int scoutCards = 0, sniperCards = 0, sergeantCards = 0;
    for (const auto& card : deck) {
        switch (card->getType()) {
        case AgentType::Scout: scoutCards++; break;
        case AgentType::Sniper: sniperCards++; break;
        case AgentType::Seargeant: sergeantCards++; break;
        }
    }

    std::cout << "Cards in deck (" << deck.size() << " total):" << std::endl;
    std::cout << "  Scout: " << scoutCards << "/4" << std::endl;
    std::cout << "  Sniper: " << sniperCards << "/3" << std::endl;
    std::cout << "  Sergeant: " << sergeantCards << "/3" << std::endl;

    // Display hand
    std::cout << "Cards in hand: " << hand.size() << std::endl;
    for (const auto& card : hand) {
        std::cout << "  ";
        card->display();
    }
}

// Player.cpp (add these methods)

Card* Player::drawTopCard() {
    if (deck.empty()) {
        return nullptr;
    }

    Card* card = deck.front();
    deck.erase(deck.begin());
    hand.push_back(card);

    return card;
}


bool Player::hasCards() const {
    return !deck.empty();
}

int Player::getDeckSize() const {
    return deck.size();
}

Unit* Player::getAgentPiece(AgentType agentType) const {
    for (const auto& piece : pieces) {
        if ((agentType == AgentType::Scout && piece->getType() == Unit::Type::SCOUT) ||
            (agentType == AgentType::Sniper && piece->getType() == Unit::Type::SNIPER) ||
            (agentType == AgentType::Seargeant && piece->getType() == Unit::Type::SERGEANT)) {
            return piece.get();
        }
    }
    return nullptr;
}

bool Player::canPerformAction(AgentType agentType, const std::string& actionType) const {
    // Basic validation - will be expanded later
    Unit* piece = getAgentPiece(agentType);
    if (!piece) return false;

    // TODO: Add more specific validations based on PDF rules
    return true;
}

bool Player::removeOneCardOfType(AgentType type)
{
    for (auto it = deck.begin(); it != deck.end(); ++it) {
        if ((*it)->getType() == type) {
            delete *it;
            deck.erase(it);
            std::cout << name << " lost a " << Card(type).getTypeName() << " card\n";
            return true;
        }
    }
    return false;
}
