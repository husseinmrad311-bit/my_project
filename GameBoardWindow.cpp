#include "GameBoardWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include "Cell.h"

// ============================================
// Constructor
// ============================================
GameBoardWindow::GameBoardWindow(QWidget *parent)
    : QWidget(parent)
{
    setWindowTitle("Undaunted - Game Board");

    m_scene = new QGraphicsScene(this);

    buildUI();
}

// ============================================
// Build UI
// ============================================
void GameBoardWindow::buildUI()
{
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // ===== Top Bar =====
    QHBoxLayout* topBar = new QHBoxLayout();

    turnLabel = new QLabel("CURRENT TURN:");
    currentPlayerLabel = new QLabel("-");
    currentCardLabel = new QLabel("Current Card: -");

    topBar->addWidget(turnLabel);
    topBar->addWidget(currentPlayerLabel);
    topBar->addStretch();
    topBar->addWidget(currentCardLabel);

    // ===== Feedback Bar =====
    QHBoxLayout* feedbackBar = new QHBoxLayout();
    feedbackLabel = new QLabel("Ready.");
    feedbackBar->addWidget(feedbackLabel);

    // ===== Action Bar =====
    QHBoxLayout* actionBar = new QHBoxLayout();

    moveButton = new QPushButton("Move");
    attackButton = new QPushButton("Attack");
    scoutButton = new QPushButton("Scout");
    controlButton = new QPushButton("Control");
    endTurnButton = new QPushButton("End Turn");

    actionBar->addWidget(moveButton);
    actionBar->addWidget(attackButton);
    actionBar->addWidget(scoutButton);
    actionBar->addWidget(controlButton);
    actionBar->addStretch();
    actionBar->addWidget(endTurnButton);

    // ===== Board View =====
    boardView = new QGraphicsView();
    boardView->setScene(m_scene);
    boardView->setRenderHint(QPainter::Antialiasing);
    boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // ===== Legend Panel =====
    QLabel* legendLabel = new QLabel();
    legendLabel->setMinimumWidth(260);
    legendLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);

    // 🔥 BLACK THEME
    legendLabel->setStyleSheet(
        "background-color: #111111;"
        "color: white;"
        "padding: 15px;"
        "font-size: 12px;"
        );

    legendLabel->setText(
        "<b style='color:white;'>LEGEND</b><br><br>"

        "<b style='color:white;'>Units:</b><br>"
        "<span style='color:red;'>S</span> = Scout A<br>"
        "<span style='color:blue;'>S</span> = Scout B<br>"
        "<span style='color:red;'>N</span> = Sniper A<br>"
        "<span style='color:blue;'>N</span> = Sniper B<br>"
        "<span style='color:red;'>G</span> = Sergeant A<br>"
        "<span style='color:blue;'>G</span> = Sergeant B<br><br>"

        "<b style='color:white;'>Control:</b><br>"
        "Thick <span style='color:red;'>Red</span> Border = Player A<br>"
        "Thick <span style='color:blue;'>Blue</span> Border = Player B<br><br>"

        "<b style='color:white;'>Mark:</b><br>"
        "<span style='color:#00ff00;'>Green Dashed</span> = Marked by A<br>"
        "<span style='color:cyan;'>Cyan Dashed</span> = Marked by B"
        );

    // ===== Center Layout (Board + Legend) =====
    QHBoxLayout* centerLayout = new QHBoxLayout();
    centerLayout->addWidget(boardView, 3);
    centerLayout->addWidget(legendLabel, 1);

    // ===== Footer =====
    QHBoxLayout* footerBar = new QHBoxLayout();
    deckInfoLabel = new QLabel("-");
    footerBar->addStretch();
    footerBar->addWidget(deckInfoLabel);
    footerBar->addStretch();

    // ===== Add to Layout =====
    mainLayout->addLayout(topBar);
    mainLayout->addLayout(feedbackBar);
    mainLayout->addLayout(actionBar);
    mainLayout->addLayout(centerLayout, 1);
    mainLayout->addLayout(footerBar);

    // ===== Connections =====
    connect(moveButton, &QPushButton::clicked,
            this, [this]() { handleAction("move"); });

    connect(attackButton, &QPushButton::clicked,
            this, [this]() { handleAction("attack"); });

    connect(scoutButton, &QPushButton::clicked,
            this, [this]() { handleAction("scout"); });

    connect(controlButton, &QPushButton::clicked,
            this, [this]() { handleAction("control"); });

    connect(endTurnButton, &QPushButton::clicked,
            this, [this]() {
                if (!m_game) return;
                m_game->nextTurn();
                updateUI();
            });
}

// ============================================
// Inject Game
// ============================================
void GameBoardWindow::setGame(Game* game)
{
    m_game = game;
    updateUI();
}

// ============================================
// Handle Actions
// ============================================
void GameBoardWindow::handleAction(const std::string& action)
{
    if (!m_game) return;

    if (m_selectedTile.isEmpty())
    {
        feedbackLabel->setText("Select a tile first!");
        return;
    }

    bool success = m_game->performAction(action,
                                         m_selectedTile.toStdString());

    feedbackLabel->setText(success ?
                               "Action Successful" :
                               "Action Failed");

    updateUI();
}

// ============================================
// Update UI
// ============================================
void GameBoardWindow::updateUI()
{
    if (!m_game) return;

    refreshHUD();
    renderBoardFromGame();
    fitBoardToView();
}

// ============================================
// Update HUD
// ============================================
void GameBoardWindow::refreshHUD()
{
    if (!m_game) return;

    Player* current = m_game->getCurrentPlayer();
    if (!current) return;

    currentPlayerLabel->setText(
        QString::fromStdString(current->getName())
        );

    if (m_game->getCurrentCard())
    {
        currentCardLabel->setText(
            "Current Card: " +
            QString::fromStdString(
                m_game->getCurrentCard()->getTypeName()
                )
            );
    }

    deckInfoLabel->setText(
        "Deck Size: " +
        QString::number(current->getDeckSize())
        );

    if (m_game->isGameOver())
        feedbackLabel->setText("GAME OVER");
}

// ============================================
// Render Board From Engine
// ============================================

void GameBoardWindow::renderBoardFromGame()
{
    if (!m_game)
        return;

    m_scene->clear();

    const Board& board = m_game->board;

    const int rows = board.rows;
    const int cols = board.cols;

    const int cellSize = 60;
    const int gap = 8;

    QVector<int> rowTileCounts(rows, 0);
    int maxTilesInAnyRow = 0;

    // -------- Count valid tiles per row --------
    for (int r = 0; r < rows; ++r)
    {
        int validCount = 0;

        for (int c = 0; c < cols; ++c)
        {
            const Cell& cell = board.grid[r][c];

            if (cell.type != -1 && !cell.tileId.isEmpty())
                validCount++;
        }

        rowTileCounts[r] = validCount;
        maxTilesInAnyRow = std::max(maxTilesInAnyRow, validCount);
    }

    // -------- Draw centered rows --------
    for (int r = 0; r < rows; ++r)
    {
        const int tilesInRow = rowTileCounts[r];

        if (tilesInRow == 0)
            continue;

        const qreal horizontalOffset =
            (maxTilesInAnyRow - tilesInRow) / 2.0;

        int tileIndex = 0;

        for (int c = 0; c < cols; ++c)
        {
            const Cell& cell = board.grid[r][c];

            if (cell.type == -1 || cell.tileId.isEmpty())
                continue;

            const qreal x =
                (horizontalOffset + tileIndex) * (cellSize + gap);

            const qreal y =
                r * (cellSize + gap);

            CellItem* item = new CellItem(cell, cellSize);

            item->setPos(x, y);

            connect(item,
                    &CellItem::cellClicked,
                    this,
                    &GameBoardWindow::onCellClicked);

            m_scene->addItem(item);

            tileIndex++;
        }
    }
}

// ============================================
// Fit Board
// ============================================
void GameBoardWindow::fitBoardToView()
{
    if (!boardView || !boardView->scene())
        return;

    QRectF r = boardView->scene()->itemsBoundingRect();
    if (r.isNull())
        return;

    boardView->setSceneRect(r);
    boardView->resetTransform();
    boardView->fitInView(r, Qt::KeepAspectRatio);
}

// ============================================
// Resize
// ============================================
void GameBoardWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    fitBoardToView();
}
void GameBoardWindow::onCellClicked(const QString& tileId)
{
    m_selectedTile = tileId;

    // Clear previous selection
    if (m_selectedItem)
        m_selectedItem->setSelected(false);

    // Find clicked item
    for (QGraphicsItem* item : m_scene->items())
    {
        CellItem* cell = dynamic_cast<CellItem*>(item);
        if (cell && cell->getTileId() == tileId)
        {
            m_selectedItem = cell;
            m_selectedItem->setSelected(true);
            break;
        }
    }

    feedbackLabel->setText("Selected: " + tileId);
}
