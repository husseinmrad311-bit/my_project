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
        SERGEANT    // seargeant
    };

    Unit(int id, int playerId, Type type);

    // simple getters
    int getId() const;
    int getPlayerId() const;
    Type getType() const;
    Cell* getPosition() const;

    // set position
    void setPosition(Cell* cell);

    // display
    std::string getTypeName() const;
    void display() const;

private:
    int id;
    int playerId;
    Type type;
    Cell* position;
};

#endif
