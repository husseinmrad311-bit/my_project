#ifndef UNIT_H
#define UNIT_H

#include <string>
#include "Cell.h"

class Unit {
public:
    // Match the spelling from Cell.h
    enum class Type {
        SCOUT,      // Scout
        SNIPER,     // Sniper
        SERGEANT    // Note: Your Cell.h has "Seargeant" but Unit uses "SERGEANT"
    };

    Unit(int id, int playerId, Type type);

    // Simple getters
    int getId() const;
    int getPlayerId() const;
    Type getType() const;
    Cell* getPosition() const;

    // Set position
    void setPosition(Cell* cell);

    // Display
    std::string getTypeName() const;
    void display() const;

private:
    int id;
    int playerId;
    Type type;
    Cell* position;
};

#endif
