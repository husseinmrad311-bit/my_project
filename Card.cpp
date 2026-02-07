#include "Card.h"
#include <iostream>

Card::Card(AgentType type) : type(type) {}

AgentType Card::getType() const { return type; }

std::string Card::getTypeName() const {
    switch (type) {
    case AgentType::Scout: return "Scout";
    case AgentType::Sniper: return "Sniper";
    case AgentType::Seargeant: return "Sergeant";  // Note: Your Cell.h has "Seargeant" not "Sergeant"
    default: return "Unknown";
    }
}

void Card::display() const {
    std::cout << "[" << getTypeName() << " Card]" << std::endl;
}
