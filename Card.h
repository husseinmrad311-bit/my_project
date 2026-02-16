#ifndef CARD_H
#define CARD_H

#include <string>
#include "Cell.h"

class Card {
public:
    Card(AgentType type);

    //getter
    AgentType getType() const;

    //get display name
    std::string getTypeName() const;

    //display
    void display() const;

private:
    AgentType type;
};

#endif
