// Unit.cpp
#include "Unit.h"
#include <iostream>

Unit::Unit(int id, int playerId, Type type)
    : id(id), playerId(playerId), type(type), position(nullptr) {}

int Unit::getId() const { return id; }
int Unit::getPlayerId() const { return playerId; }
Unit::Type Unit::getType() const { return type; }
Cell* Unit::getPosition() const { return position; }

// MODIFIED: Add validation in setPosition
void Unit::setPosition(Cell* cell)
{
    // Optional debug - remove if too verbose
    if (cell) {
        std::cout << "Unit " << getTypeName() << " moving to cell: "
                  << cell->tileId.toStdString() << std::endl;
    } else {
        std::cout << "Unit " << getTypeName() << " position set to NULL" << std::endl;
    }

    position = cell;
}

std::string Unit::getTypeName() const {
    switch (type) {
    case Unit::Type::SCOUT: return "Scout";
    case Unit::Type::SNIPER: return "Sniper";
    case Unit::Type::SERGEANT: return "Sergeant";
    default: return "Unknown";
    }
}

void Unit::display() const {
    std::cout << getTypeName() << " [P" << playerId << "]";
    if (position) {
        std::cout << " at " << position->getCoordinate();
    }
    std::cout << std::endl;
}
