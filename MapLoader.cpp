// MapLoader.cpp
// Parses map layout + state file

#include "MapLoader.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QFileInfo>
#include <QRegularExpression>
#include <iostream>

// --------------------------------------------------
// Layout parsing
// --------------------------------------------------

static bool parseCellSegment(const QString& seg, Cell& outCell)
{
    const QString s = seg.trimmed();

    if (s.isEmpty()) {
        outCell.tileId.clear();
        outCell.type = -1;
        return true;
    }

    static const QRegularExpression re(R"(^([A-Za-z]\d{2})\s*:\s*(-?\d+)$)");
    const QRegularExpressionMatch m = re.match(s);

    if (!m.hasMatch())
        return false;

    outCell.tileId = m.captured(1).toUpper();
    outCell.type   = m.captured(2).toInt();
    return true;
}

// --------------------------------------------------
// Helpers
// --------------------------------------------------

static Side parseSide(const QString& s)
{
    if (s == "A") return Side::A;
    if (s == "B") return Side::B;
    return Side::None;
}

static AgentType parseAgentType(const QString& s)
{
    if (s == "Scout")     return AgentType::Scout;
    if (s == "Sniper")    return AgentType::Sniper;
    if (s == "Seargeant") return AgentType::Seargeant;
    return AgentType::None;
}

// --------------------------------------------------
// Main loader
// --------------------------------------------------

bool MapLoader::loadFromFile(const QString& filePath,
                             MapData& out,
                             QString& errorMessage)
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

    static const QRegularExpression leadingNumber(R"(^\s*\d+\s*)");

    // --------------------------------------------------
    // Parse Layout
    // --------------------------------------------------

    while (!in.atEnd()) {
        QString line = in.readLine();

        if (!line.contains('|') || !line.contains(':'))
            continue;

        line.remove(leadingNumber);

        QStringList parts = line.split('|', Qt::KeepEmptyParts);

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
                c.tileId.clear();
                c.type = -1;
            }
            rowCells.push_back(c);
        }

        maxCols = qMax(maxCols, rowCells.size());
        parsedRows.push_back(rowCells);
    }

    if (parsedRows.isEmpty() || maxCols <= 0) {
        errorMessage = "No map data found.";
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
            cell.type = -1;

            if (c < parsedRows[r].size())
                cell = parsedRows[r][c];

            board.grid[r][c] = cell;
        }
    }

    board.buildNeighbors();

    // --------------------------------------------------
    // Store map info without loading state
    // --------------------------------------------------

    QFileInfo info(filePath);
    out.mapName = info.baseName();
    out.description.clear();
    out.board = std::move(board);
    out.stateFilePath = "";  // Will be set separately when loading state

    return true;
}

//Load state file separately
bool MapLoader::loadStateFile(const QString& stateFilePath, Board& board, QString& errorMessage)
{
    errorMessage.clear();

    std::cout << "\n=== LOADING STATE FILE: " << stateFilePath.toStdString() << " ===\n";

    QFile stateFile(stateFilePath);
    if (!stateFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        errorMessage = "Cannot open state file:\n" + stateFilePath;
        return false;
    }

    // FIRST: Clear all existing agents from the board
    std::cout << "Clearing all existing agents from board...\n";
    for (int r = 0; r < board.rows; ++r) {
        for (int c = 0; c < board.cols; ++c) {
            board.grid[r][c].clearAgent();
            board.grid[r][c].marked = false;
            board.grid[r][c].markedBy = Side::None;
            board.grid[r][c].controlledBy = Side::None;
        }
    }

    QTextStream sin(&stateFile);
    int agentsPlaced = 0;
    int marksPlaced = 0;
    int controlsPlaced = 0;

    while (!sin.atEnd())
    {
        QString line = sin.readLine().trimmed();
        if (line.isEmpty() || line.startsWith('#'))
            continue;

        // Format: A03:A,Scout
        QStringList parts = line.split(':');
        if (parts.size() != 2) {
            std::cout << "  WARNING: Invalid line format: " << line.toStdString() << std::endl;
            continue;
        }

        QString cellId = parts[0].trimmed().toUpper();
        QStringList rhs = parts[1].split(',');
        if (rhs.size() != 2) {
            std::cout << "  WARNING: Invalid RHS format: " << parts[1].toStdString() << std::endl;
            continue;
        }

        Side parsedSide = parseSide(rhs[0].trimmed());
        QString typeStr = rhs[1].trimmed();

        bool found = false;
        for (int r = 0; r < board.rows; ++r)
        {
            for (int c = 0; c < board.cols; ++c)
            {
                Cell& cell = board.grid[r][c];

                if (cell.tileId.trimmed().toUpper() == cellId)
                {
                    found = true;

                    // MARK
                    if (typeStr.compare("Mark", Qt::CaseInsensitive) == 0)
                    {
                        cell.marked = true;
                        cell.markedBy = parsedSide;
                        marksPlaced++;
                        std::cout << "  Marked " << cellId.toStdString()
                                  << " for " << (parsedSide == Side::A ? "A" : "B") << std::endl;
                    }
                    // CONTROL
                    else if (typeStr.compare("Control", Qt::CaseInsensitive) == 0)
                    {
                        cell.controlledBy = parsedSide;
                        controlsPlaced++;
                        std::cout << "  Controlled " << cellId.toStdString()
                                  << " by " << (parsedSide == Side::A ? "A" : "B") << std::endl;
                    }
                    // AGENT
                    else
                    {
                        AgentType agent = parseAgentType(typeStr);
                        if (agent != AgentType::None)
                        {
                            // Only place if cell is empty
                            if (cell.agent == AgentType::None) {
                                cell.agent = agent;
                                cell.agentSide = parsedSide;
                                agentsPlaced++;
                                std::cout << "  Placed " << typeStr.toStdString()
                                          << " for " << (parsedSide == Side::A ? "A" : "B")
                                          << " at " << cellId.toStdString() << std::endl;
                            } else {
                                std::cout << "  ERROR: Cell " << cellId.toStdString()
                                << " already has an agent! Skipping.\n";
                            }
                        }
                    }
                    break;
                }
            }
            if (found) break;
        }

        if (!found) {
            std::cout << "  WARNING: Cell " << cellId.toStdString() << " not found in board!\n";
        }
    }

    std::cout << "State loading complete: "
              << agentsPlaced << " agents, "
              << marksPlaced << " marks, "
              << controlsPlaced << " controls placed.\n";
    std::cout << "========================================\n";

    return true;
}
