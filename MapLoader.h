#ifndef MAPLOADER_H
#define MAPLOADER_H

#include "Board.h"
#include <QString>

struct MapData {
    QString mapName;
    QString description;
    Board board;
};

class MapLoader {
public:
    //parses a map text file and fills MapData.
    //returns true on success; otherwise false and sets errorMessage.
    static bool loadFromFile(const QString& filePath, MapData& out, QString& errorMessage);
};

#endif
