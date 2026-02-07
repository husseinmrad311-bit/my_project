#ifndef CARD_H
#define CARD_H

#include <string>
#include "Cell.h"  // Use AgentType from Cell.h

class Card {
public:
    Card(AgentType type);

    // Simple getter
    AgentType getType() const;

    // Get display name
    std::string getTypeName() const;

    // Display
    void display() const;

private:
    AgentType type;
};

#endif
