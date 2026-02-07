#include "GameBoardWindow.h"
#include "ui_GameBoardWindow.h"

#include "CellItem.h"
#include "Cell.h"   //  for Side / AgentType

#include <QTimer>
#include <QDebug>
#include <QMessageBox>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>

GameBoardWindow::GameBoardWindow(QWidget *parent)
    : QWidget(parent),
    ui(new Ui::GameBoardWindow)
{
    ui->setupUi(this);
    ui->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setAlignment(Qt::AlignCenter);

    setWindowTitle("Undaunted - Game Board");

    m_scene = new QGraphicsScene(this);
    ui->boardView->setScene(m_scene);

    ui->boardView->setRenderHint(QPainter::Antialiasing, true);
}

GameBoardWindow::~GameBoardWindow()
{
    delete ui;
}

void GameBoardWindow::setPlayerNames(const QString& p1, const QString& p2)
{
    ui->player1Label->setText(p1);
    ui->player2Label->setText(p2);
}

bool GameBoardWindow::loadAndShowMap(const QString& mapPath, QString& error)
{
    MapData data;
    if (!MapLoader::loadFromFile(mapPath, data, error)) {
        return false;
    }

    if (ui->mapLabel)
        ui->mapLabel->setText(data.mapName);

    renderBoard(data);

    QTimer::singleShot(0, this, [this]() {
        fitBoardToView();
    });

    return true;
}

void GameBoardWindow::renderBoard(const MapData& data)
{
    m_scene->clear();

    const int rows = data.board.rows;
    const int cols = data.board.cols;

    const int cellSize = 60;
    const int gap = 8;

    QVector<int> rowTileCounts(rows, 0);
    int maxTilesInAnyRow = 0;

    for (int r = 0; r < rows; ++r) {
        int cnt = 0;
        for (int c = 0; c < cols; ++c) {
            const Cell& cell = data.board.grid[r][c];
            if (cell.type != -1 && !cell.tileId.isEmpty())
                cnt++;
        }
        rowTileCounts[r] = cnt;
        maxTilesInAnyRow = qMax(maxTilesInAnyRow, cnt);
    }

    for (int r = 0; r < rows; ++r) {

        const int tilesThisRow = rowTileCounts[r];
        const qreal rowOffsetTiles =
            (maxTilesInAnyRow - tilesThisRow) / 2.0;

        int tileIndexInRow = 0;

        for (int c = 0; c < cols; ++c) {
            const Cell& cell = data.board.grid[r][c];

            if (cell.type == -1 || cell.tileId.isEmpty())
                continue;

            const qreal x =
                (rowOffsetTiles + tileIndexInRow) * (cellSize + gap);
            const qreal y =
                r * (cellSize + gap);

            auto* cellItem =
                new CellItem(cell.tileId, cell.type, cellSize);

            cellItem->setPos(x, y);

            // 🔴 TEMPORARY VISUAL TEST (Day 2 validation ONLY)
            if ((r + c) % 3 == 0)
                cellItem->setMarked(true);

            if ((r + c) % 3 == 1)
                cellItem->setControlledBy(Side::A);   // ✅ FIXED

            if ((r + c) % 3 == 2)
                cellItem->setAgent(AgentType::Scout);

            cellItem->setToolTip(
                cell.tileId + " : " + QString::number(cell.type));

            m_scene->addItem(cellItem);

            tileIndexInRow++;
        }
    }

    ui->boardView->fitInView(
        m_scene->itemsBoundingRect(),
        Qt::KeepAspectRatio
        );
}

void GameBoardWindow::fitBoardToView()
{
    if (!ui->boardView || !ui->boardView->scene())
        return;

    QRectF r = ui->boardView->scene()->itemsBoundingRect();
    if (r.isNull())
        return;

    ui->boardView->setSceneRect(r);

    ui->boardView->resetTransform();
    ui->boardView->fitInView(r, Qt::KeepAspectRatio);
}

void GameBoardWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    fitBoardToView();
}
