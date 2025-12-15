// ===== FILE: MapLoader.cpp =====
#include "MapLoader.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>
#include <QRegularExpression>

static bool parseCellSegment(const QString& seg, Cell& outCell)
{
    const QString s = seg.trimmed();

    // empty segment => spacing
    if (s.isEmpty()) {
        outCell.tileId.clear();
        outCell.type = -1;
        return true;
    }

    // expects: A01:0 , B07:2 , etc (supports -1 too)
    static const QRegularExpression re(R"(^([A-Za-z]\d{2})\s*:\s*(-?\d+)$)");
    const QRegularExpressionMatch m = re.match(s);
    if (!m.hasMatch())
        return false;

    outCell.tileId = m.captured(1).toUpper();
    outCell.type   = m.captured(2).toInt();
    return true;
}

bool MapLoader::loadFromFile(const QString& filePath, MapData& out, QString& errorMessage)
{
    errorMessage.clear();
    out = MapData{};

    QFile f(filePath);
    if (!f.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorMessage = "Cannot open map file:\n" + filePath;
        return false;
    }

    QTextStream in(&f);

    QList<QList<Cell>> parsedRows;
    int maxCols = 0;

    // Removes optional line number prefix: "1 |A01:0|..." or "1|A01:0|..."
    static const QRegularExpression leadingNumber(R"(^\s*\d+\s*)");

    while (!in.atEnd()) {
        QString line = in.readLine();

        // if the line doesn't look like map data, skip
        if (!line.contains('|') || !line.contains(':'))
            continue;

        // remove optional "row number" at start
        line.remove(leadingNumber);

        // split by '|' and KEEP empty parts (important)
        QStringList parts = line.split('|', Qt::KeepEmptyParts);

        // If the line starts/ends with '|', Qt gives empty first/last segment.
        // Remove ONLY ONE empty at start/end (the framing pipes), but keep real spacing.
        if (!parts.isEmpty() && parts.first().trimmed().isEmpty())
            parts.removeFirst();
        if (!parts.isEmpty() && parts.last().trimmed().isEmpty())
            parts.removeLast();

        if (parts.isEmpty())
            continue;

        QList<Cell> rowCells;
        rowCells.reserve(parts.size());

        for (const QString& seg : parts) {
            Cell c;
            if (!parseCellSegment(seg, c)) {
                // bad segment => treat as spacing
                c.tileId.clear();
                c.type = -1;
            }
            rowCells.push_back(c);
        }

        maxCols = qMax(maxCols, rowCells.size());
        parsedRows.push_back(rowCells);
    }

    if (parsedRows.isEmpty() || maxCols <= 0) {
        errorMessage = "No map data found. Expected rows like:\n|A01:0|A02:1| |A05:1|";
        return false;
    }

    Board board;
    board.rows = parsedRows.size();
    board.cols = maxCols;
    board.grid.assign(board.rows, std::vector<Cell>(board.cols));

    for (int r = 0; r < board.rows; ++r) {
        for (int c = 0; c < board.cols; ++c) {
            Cell cell;
            cell.tileId.clear();
            cell.type = -1; // default spacing
            if (c < parsedRows[r].size())
                cell = parsedRows[r][c];

            board.grid[r][c] = cell;
        }
    }

    out.mapName = QFileInfo(filePath).baseName();
    out.description.clear();
    out.board = board;
    return true;
}
