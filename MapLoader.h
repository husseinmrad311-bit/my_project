#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "Board.h"
#include <QString>

// ================================
// MapData (layout + state)
// ================================

struct MapData {
    QString mapName;
    QString description;
    Board board;
    QString stateFilePath;  // Path to state file
};

// ================================
// MapLoader
// ================================

class MapLoader {
public:
    // Parses a map layout text file and fills MapData.
    static bool loadFromFile(const QString& filePath,
                             MapData& out,
                             QString& errorMessage);

    // NEW: Load state file into an existing board
    static bool loadStateFile(const QString& stateFilePath,
                              Board& board,
                              QString& errorMessage);
};

#endif // MAPLOADER_H
