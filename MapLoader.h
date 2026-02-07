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

    // Phase 2: optional path to state file
    QString stateFilePath;
};

// ================================
// MapLoader
// ================================

class MapLoader {
public:
    // Parses a map layout text file and fills MapData.
    // Returns true on success; otherwise false and sets errorMessage.
    static bool loadFromFile(const QString& filePath,
                             MapData& out,
                             QString& errorMessage);
};

#endif // MAPLOADER_H
