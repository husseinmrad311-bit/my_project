// ===== FILE: MapLoader.cpp =====
#include "MapLoader.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>

static bool parseSizeLine(const QString& line, int& rows, int& cols) {
    // "SIZE: 6x6"
    QString s = line;
    int colonPos = s.indexOf(':');
    if (colonPos < 0) return false;
    s = s.mid(colonPos + 1).trimmed(); // "6x6"
    const QStringList parts = s.split('x', Qt::SkipEmptyParts);
    if (parts.size() != 2) return false;
    bool ok1=false, ok2=false;
    rows = parts[0].trimmed().toInt(&ok1);
    cols = parts[1].trimmed().toInt(&ok2);
    return ok1 && ok2 && rows > 0 && cols > 0;
}

static bool parseCellToken(const QString& token, Cell& cell) {
    // token example: "A01:1"
    QString t = token.trimmed();
    if (t.isEmpty()) return false;

    // remove leading/trailing pipes if user wrote "|A01:1|"
    if (t.startsWith('|')) t = t.mid(1).trimmed();
    if (t.endsWith('|'))   t = t.left(t.size()-1).trimmed();

    if (t.isEmpty()) return false;

    const int colon = t.indexOf(':');
    if (colon <= 0) return false;

    const QString id = t.left(colon).trimmed();
    const QString typeStr = t.mid(colon + 1).trimmed();

    bool ok=false;
    const int type = typeStr.toInt(&ok);
    if (!ok) return false;

    cell.tileId = id;
    cell.type = type;
    return true;
}

static bool lineLooksLikeData(const QString& line) {
    // If it contains ':' and '|' it's almost certainly map data
    return line.contains(':') && line.contains('|');
}

bool MapLoader::loadFromFile(const QString& filePath, MapData& out, QString& errorMessage) {
    errorMessage.clear();
    out = MapData{};

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorMessage = "Cannot open map file:\n" + filePath;
        return false;
    }

    QTextStream in(&f);

    QString mapName, desc;
    int rows = 0, cols = 0;

    bool inData = false;                 // for header-based maps
    QList<QList<Cell>> parsedRows;        // actual parsed grid rows
    bool sawAnyDataRow = false;

    while (!in.atEnd()) {
        QString line = in.readLine().trimmed();
        if (line.isEmpty()) continue;

        // If no header exists, switch to data mode as soon as we see a data-looking line
        if (!inData && lineLooksLikeData(line)) {
            inData = true;
        }

        if (!inData) {
            if (line.startsWith("MAP NAME:", Qt::CaseInsensitive)) {
                mapName = line.mid(line.indexOf(':') + 1).trimmed();
            } else if (line.startsWith("DESCRIPTION:", Qt::CaseInsensitive)) {
                desc = line.mid(line.indexOf(':') + 1).trimmed();
            } else if (line.startsWith("SIZE:", Qt::CaseInsensitive)) {
                if (!parseSizeLine(line, rows, cols)) {
                    errorMessage = "Invalid SIZE line. Expected: SIZE: RxC (e.g., SIZE: 6x6)";
                    return false;
                }
            } else if (line.startsWith("DATA:", Qt::CaseInsensitive)) {
                inData = true;
            }
            continue;
        }

        // --- DATA parsing (your real maps) ---
        // Some of your files start with a row number like: "1 |A01:0 |A02:1 ..."
        // Remove a leading integer token if present:
        QString working = line;

        // Split by '|', keep empties away
        QStringList tokens = working.split('|', Qt::KeepEmptyParts);

        // Clean tokens and if first token is just a number -> drop it
        // Example tokens[0] might be "1 " or " 2 "
        if (!tokens.isEmpty()) {
            const QString first = tokens[0].trimmed();
            bool okNum = false;
            first.toInt(&okNum);
            if (okNum) {
                tokens.pop_front();
            }
        }

        QList<Cell> rowCells;
        for (const QString& token : tokens) {
            Cell c;
            if (parseCellToken(token, c)) {
                rowCells.push_back(c);
            }
        }

        if (!rowCells.isEmpty()) {
            parsedRows.push_back(rowCells);
            sawAnyDataRow = true;
        }
    }

    if (mapName.isEmpty())
        mapName = QFileInfo(filePath).baseName();

    if (!sawAnyDataRow) {
        errorMessage = "No map cell data found in file:\n" + filePath;
        return false;
    }

    // If SIZE wasn't provided, infer it from parsed data
    if (rows <= 0) rows = parsedRows.size();

    if (cols <= 0) {
        int maxCols = 0;
        for (const auto& r : parsedRows)
            maxCols = std::max(maxCols, (int)r.size());
        cols = maxCols;
    }

    if (rows <= 0 || cols <= 0) {
        errorMessage = "Could not infer map size from file:\n" + filePath;
        return false;
    }

    // Build board grid rows x cols (pad missing)
    Board board;
    board.rows = rows;
    board.cols = cols;
    board.grid.assign(rows, std::vector<Cell>(cols));

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            Cell cell;
            if (r < parsedRows.size() && c < parsedRows[r].size()) {
                cell = parsedRows[r][c];
            } else {
                cell.tileId = QString("%1%2").arg(QChar('A' + r)).arg(c + 1, 2, 10, QChar('0'));
                cell.type = 0;
            }
            board.grid[r][c] = cell;
        }
    }

    out.mapName = mapName;
    out.description = desc;
    out.board = board;
    return true;
}
