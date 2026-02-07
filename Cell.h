#ifndef CELL_H
#define CELL_H

#include <QString>
#include <vector>
#include <string>
#include <iostream>

// ================================
// Phase 2 enums (DATA ONLY)
// ================================

enum class Side {
    None,
    A,
    B
};

// Agent types - using YOUR exact naming from first chat
enum class AgentType {
    None,
    Scout,      // ✓ Your preferred name
    Sniper,     // ✓ Your preferred name
    Seargeant   // Note: Keeping your spelling "Seargeant"
};

// ================================
// Cell (pure data container)
// ================================

class Cell {
public:
    // ----- Phase 1 (unchanged) -----
    QString tileId;   // e.g. "A01"
    int type = 0;     // terrain type (0,1,2,...)
    std::vector<Cell*> neighbors;

    // ----- Phase 2 (Day 2 parsing state) -----
    bool marked = false;
    Side markedBy = Side::None;
    Side controlledBy = Side::None;
    AgentType agent = AgentType::None;
    Side agentSide = Side::None;

    // ----- New methods for Phase 2 Core Game -----

    // Get coordinate string (QString to std::string conversion)
    std::string getCoordinate() const {
        return tileId.toStdString();
    }

    // Get grid coordinates (parse from tileId like "A01")
    int getX() const {
        if (tileId.isEmpty()) return -1;
        // Column letter to number (A=0, B=1, etc.)
        QChar firstChar = tileId[0];
        return firstChar.toLatin1() - 'A';
    }

    int getY() const {
        if (tileId.isEmpty()) return -1;
        // Row number (01 = 0, 02 = 1, etc.)
        bool ok;
        int row = tileId.mid(1).toInt(&ok);
        return ok ? row - 1 : -1;
    }

    // Check if cell is occupied by any agent
    bool isOccupied() const {
        return agent != AgentType::None;
    }

    // Check if cell is occupied by specific player
    bool isOccupiedBy(Side side) const {
        return agent != AgentType::None && agentSide == side;
    }

    // Clear agent from cell
    void clearAgent() {
        agent = AgentType::None;
        agentSide = Side::None;
    }

    // Set agent in cell
    void setAgent(AgentType agentType, Side side) {
        agent = agentType;
        agentSide = side;
    }

    // Get agent type name as string
    QString getAgentTypeName() const {
        switch (agent) {
        case AgentType::Scout: return "Scout";
        case AgentType::Sniper: return "Sniper";
        case AgentType::Seargeant: return "Sergeant"; // Fixed spelling for display
        default: return "None";
        }
    }

    // Get side name as string
    QString getSideName() const {
        switch (agentSide) {
        case Side::A: return "A";
        case Side::B: return "B";
        default: return "None";
        }
    }

    // Display cell info for debugging
    void displayInfo() const {
        QString info = QString("Cell %1: ").arg(tileId);
        if (isOccupied()) {
            info += QString("Occupied by %1 (Side %2)").arg(getAgentTypeName()).arg(getSideName());
        } else {
            info += "Empty";
        }
        // Convert to std::string for console output
        std::cout << info.toStdString() << std::endl;
    }
};

#endif // CELL_H
