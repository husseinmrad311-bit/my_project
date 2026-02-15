#include "Player.h"
#include <algorithm>
#include <random>
#include <iostream>

// --------------------------------------------------
// CONSTRUCTORS
// --------------------------------------------------

Player::Player(int playerId)
{
    id = playerId;
    name = "Player " + std::to_string(playerId);

    initializeCards();
    initializePieces();
}

Player::Player(int playerId, const std::string& playerName)
{
    id = playerId;
    name = playerName;

    initializeCards();
    initializePieces();
}

// --------------------------------------------------
// GETTERS
// --------------------------------------------------

int Player::getId() const { return id; }
const std::string& Player::getName() const { return name; }
const std::vector<std::unique_ptr<Unit>>& Player::getPieces() const { return pieces; }

// --------------------------------------------------
// Creation helpers
// --------------------------------------------------

Card* Player::createCard(AgentType type) {
    return new Card(type);
}

std::unique_ptr<Unit> Player::createPiece(AgentType type) {
    Unit::Type unitType;

    switch (type) {
    case AgentType::Scout:     unitType = Unit::Type::SCOUT; break;
    case AgentType::Sniper:    unitType = Unit::Type::SNIPER; break;
    case AgentType::Seargeant: unitType = Unit::Type::SERGEANT; break;
    default:                   unitType = Unit::Type::SCOUT;
    }

    int pieceId = (id * 10) + static_cast<int>(type);
    return std::make_unique<Unit>(pieceId, id, unitType);
}

// --------------------------------------------------
// Initialization
// --------------------------------------------------

void Player::initializeCards() {
    deck.clear();

    for (int i = 0; i < 4; i++)
        deck.push_back(createCard(AgentType::Scout));

    for (int i = 0; i < 3; i++)
        deck.push_back(createCard(AgentType::Sniper));

    for (int i = 0; i < 3; i++)
        deck.push_back(createCard(AgentType::Seargeant));

    shuffleDeck();
}

void Player::shuffleDeck() {
    if (deck.empty()) return;

    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(deck.begin(), deck.end(), g);
}

void Player::initializePieces() {
    pieces.clear();

    pieces.push_back(createPiece(AgentType::Scout));
    pieces.push_back(createPiece(AgentType::Sniper));
    pieces.push_back(createPiece(AgentType::Seargeant));
}

// --------------------------------------------------
// Card operations
// --------------------------------------------------

Card* Player::drawCard() {
    if (deck.empty()) return nullptr;

    Card* card = deck.front();
    deck.erase(deck.begin());
    hand.push_back(card);
    return card;
}

void Player::returnCardToBottom(Card* card) {
    auto it = std::find(hand.begin(), hand.end(), card);
    if (it != hand.end())
        hand.erase(it);

    deck.push_back(card);
}

// --------------------------------------------------
// Queries
// --------------------------------------------------

Unit* Player::getPiece(AgentType type) const {
    return getAgentPiece(type);
}

Unit* Player::getAgentPiece(AgentType agentType) const {
    for (const auto& piece : pieces) {
        if ((agentType == AgentType::Scout     && piece->getType() == Unit::Type::SCOUT) ||
            (agentType == AgentType::Sniper    && piece->getType() == Unit::Type::SNIPER) ||
            (agentType == AgentType::Seargeant && piece->getType() == Unit::Type::SERGEANT)) {
            return piece.get();
        }
    }
    return nullptr;
}

bool Player::hasCards() const {
    return !deck.empty();
}

int Player::getDeckSize() const {
    return deck.size();
}

bool Player::canPerformAction(AgentType agentType, const std::string&) const {
    return getAgentPiece(agentType) != nullptr;
}

// --------------------------------------------------
// Display
// --------------------------------------------------

void Player::displayStatus() const {
    std::cout << "\n=== " << name << " ===\n";

    std::cout << "Pieces:\n";
    for (const auto& piece : pieces) {
        piece->display();
    }

    int scout = 0, sniper = 0, sergeant = 0;

    for (const auto& card : deck) {
        switch (card->getType()) {
        case AgentType::Scout: scout++; break;
        case AgentType::Sniper: sniper++; break;
        case AgentType::Seargeant: sergeant++; break;
        }
    }

    std::cout << "Cards in deck (" << deck.size() << " total):\n";
    std::cout << "  Scout: " << scout << "\n";
    std::cout << "  Sniper: " << sniper << "\n";
    std::cout << "  Sergeant: " << sergeant << "\n";
}

// --------------------------------------------------
// STEP 2 CORE LOGIC
// --------------------------------------------------

int Player::countCardsOfType(AgentType type) const {
    int count = 0;
    for (const auto& card : deck) {
        if (card->getType() == type)
            count++;
    }
    return count;
}

bool Player::removeOneCardOfType(AgentType type)
{
    for (auto it = deck.begin(); it != deck.end(); ++it) {
        if ((*it)->getType() == type) {

            delete *it;
            deck.erase(it);

            int remaining = countCardsOfType(type);

            std::cout << name << " loses one "
                      << Card(type).getTypeName()
                      << " card (" << remaining << " remaining)\n";

            if (remaining == 0) {
                removeAgentPieceIfExists(type);
            }

            return true;
        }
    }
    return false;
}

void Player::removeAgentPieceIfExists(AgentType type)
{
    for (auto it = pieces.begin(); it != pieces.end(); ++it)
    {
        bool match =
            (type == AgentType::Scout     && (*it)->getType() == Unit::Type::SCOUT) ||
            (type == AgentType::Sniper    && (*it)->getType() == Unit::Type::SNIPER) ||
            (type == AgentType::Seargeant && (*it)->getType() == Unit::Type::SERGEANT);

        if (match)
        {
            Unit* unit = it->get();

            // 🔥 Clear board cell if unit still placed
            if (unit->getPosition())
            {
                unit->getPosition()->clearAgent();
            }

            std::cout << name << "'s "
                      << Card(type).getTypeName()
                      << " unit has been removed from the board!\n";

            pieces.erase(it);
            return;
        }
    }
}
