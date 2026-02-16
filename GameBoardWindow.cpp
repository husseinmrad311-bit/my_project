#include "GameBoardWindow.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QPainter>
#include "Cell.h"
#include <QMessageBox>
#include<QApplication>
#include <windows.h>
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
// Build UI with Enhanced Stats Panel
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

    // ===== Feedback Bar (Enhanced) =====
    QVBoxLayout* feedbackBar = new QVBoxLayout();
    feedbackLabel = new QLabel("Ready.");
    feedbackLabel->setStyleSheet("font-weight: bold; color: #4CAF50;");

    actionFeedbackLabel = new QLabel("");
    actionFeedbackLabel->setStyleSheet("color: #FF9800; font-style: italic;");
    actionFeedbackLabel->setWordWrap(true);

    feedbackBar->addWidget(feedbackLabel);
    feedbackBar->addWidget(actionFeedbackLabel);

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

    // ===== Right Panel (Legend + Stats) =====
    QWidget* rightPanel = new QWidget();
    QVBoxLayout* rightLayout = new QVBoxLayout(rightPanel);
    rightPanel->setMinimumWidth(300);
    rightPanel->setMaximumWidth(350);

    // Legend
    QLabel* legendLabel = new QLabel();
    legendLabel->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    legendLabel->setStyleSheet(
        "background-color: #1e1e1e;"
        "color: white;"
        "padding: 15px;"
        "font-size: 12px;"
        "border-radius: 5px;"
        "border: 1px solid #333;"
        );

    legendLabel->setText(
        "<b style='color:#4CAF50; font-size:14px;'>LEGEND</b><br><br>"
        "<b style='color:white;'>Units:</b><br>"
        "<span style='color:#ff6b6b;'>S</span> = Scout A<br>"
        "<span style='color:#4d9eff;'>S</span> = Scout B<br>"
        "<span style='color:#ff6b6b;'>N</span> = Sniper A<br>"
        "<span style='color:#4d9eff;'>N</span> = Sniper B<br>"
        "<span style='color:#ff6b6b;'>G</span> = Sergeant A<br>"
        "<span style='color:#4d9eff;'>C</span> = Sergeant B<br><br>"
        "<b style='color:white;'>Control:</b><br>"
        "Thick <span style='color:#ff6b6b;'>Red Border</span> = Player A<br>"
        "Thick <span style='color:#4d9eff;'>Blue Border</span> = Player B<br><br>"
        "<b style='color:white;'>Mark:</b><br>"
        "<span style='color:#51cf66;'>Green Dashed</span> = Marked by A<br>"
        "<span style='color:#22b8cf;'>Cyan Dashed</span> = Marked by B"
        );

    // Stats Panel
    QLabel* statsTitle = new QLabel("PLAYER STATS");
    statsTitle->setStyleSheet(
        "background-color: #2d2d2d;"
        "color: #4CAF50;"
        "font-weight: bold;"
        "font-size: 14px;"
        "padding: 8px;"
        "border-radius: 5px 5px 0 0;"
        "border: 1px solid #444;"
        "border-bottom: none;"
        );
    statsTitle->setAlignment(Qt::AlignCenter);

    QWidget* statsWidget = new QWidget();
    statsWidget->setStyleSheet(
        "background-color: #1e1e1e;"
        "color: white;"
        "padding: 10px;"
        "border-radius: 0 0 5px 5px;"
        "border: 1px solid #444;"
        "border-top: none;"
        );

    QVBoxLayout* statsLayout = new QVBoxLayout(statsWidget);

    player1StatsLabel = new QLabel("Player 1: Loading...");
    player2StatsLabel = new QLabel("Player 2: Loading...");
    controlledTilesLabel = new QLabel("Controlled Tiles: Loading...");

    player1StatsLabel->setStyleSheet("color: #4d9eff; font-size: 12px; padding: 2px;");
    player2StatsLabel->setStyleSheet("color: #ff6b6b; font-size: 12px; padding: 2px;");
    controlledTilesLabel->setStyleSheet("color: #ffd43b; font-size: 12px; padding: 2px; border-top: 1px solid #444; margin-top: 5px; padding-top: 5px;");

    statsLayout->addWidget(player1StatsLabel);
    statsLayout->addWidget(player2StatsLabel);
    statsLayout->addWidget(controlledTilesLabel);
    statsLayout->addStretch();

    rightLayout->addWidget(legendLabel);
    rightLayout->addWidget(statsTitle);
    rightLayout->addWidget(statsWidget);
    rightLayout->addStretch();

    // ===== Center Layout (Board + Right Panel) =====
    QHBoxLayout* centerLayout = new QHBoxLayout();
    centerLayout->addWidget(boardView, 2);
    centerLayout->addWidget(rightPanel, 1);

    // ===== Footer =====
    QHBoxLayout* footerBar = new QHBoxLayout();
    deckInfoLabel = new QLabel("-");
    footerBar->addStretch();
    footerBar->addWidget(deckInfoLabel);
    footerBar->addStretch();

    // ===== Add to Main Layout =====
    mainLayout->addLayout(topBar);
    mainLayout->addLayout(feedbackBar);
    mainLayout->addLayout(actionBar);
    mainLayout->addLayout(centerLayout, 1);
    mainLayout->addLayout(footerBar);

    // ===== Connections =====
    connect(moveButton, &QPushButton::clicked, this, [this]() { handleAction("move"); });
    connect(attackButton, &QPushButton::clicked, this, [this]() { handleAction("attack"); });
    connect(scoutButton, &QPushButton::clicked, this, [this]() { handleAction("scout"); });
    connect(controlButton, &QPushButton::clicked, this, [this]() { handleAction("control"); });
    connect(endTurnButton, &QPushButton::clicked, this, [this]() {
        if (!m_game) return;
        m_game->nextTurn();
        updateUI();
    });
}

void GameBoardWindow::updateStatsPanel()
{
    if (!m_game) return;

    Player* p1 = m_game->players[0].get();
    Player* p2 = m_game->players[1].get();

    // Count cards for each player
    int p1Scout = 0, p1Sniper = 0, p1Sergeant = 0;
    int p2Scout = 0, p2Sniper = 0, p2Sergeant = 0;

    for (const auto& card : p1->deck) {
        switch (card->getType()) {
        case AgentType::Scout: p1Scout++; break;
        case AgentType::Sniper: p1Sniper++; break;
        case AgentType::Seargeant: p1Sergeant++; break;
        default: break;
        }
    }

    for (const auto& card : p2->deck) {
        switch (card->getType()) {
        case AgentType::Scout: p2Scout++; break;
        case AgentType::Sniper: p2Sniper++; break;
        case AgentType::Seargeant: p2Sergeant++; break;
        default: break;
        }
    }

    // Check if pieces are still on board
    bool p1ScoutAlive = (p1->getAgentPiece(AgentType::Scout) != nullptr);
    bool p1SniperAlive = (p1->getAgentPiece(AgentType::Sniper) != nullptr);
    bool p1SergeantAlive = (p1->getAgentPiece(AgentType::Seargeant) != nullptr);

    bool p2ScoutAlive = (p2->getAgentPiece(AgentType::Scout) != nullptr);
    bool p2SniperAlive = (p2->getAgentPiece(AgentType::Sniper) != nullptr);
    bool p2SergeantAlive = (p2->getAgentPiece(AgentType::Seargeant) != nullptr);

    // Format player stats
    QString p1Text = QString(
                         "<b>Player 1</b><br>"
                         "• Scout: %1 cards %2<br>"
                         "• Sniper: %3 cards %4<br>"
                         "• Sergeant: %5 cards %6"
                         ).arg(p1Scout)
                         .arg(p1ScoutAlive ? "🟢" : "💀")
                         .arg(p1Sniper)
                         .arg(p1SniperAlive ? "🟢" : "💀")
                         .arg(p1Sergeant)
                         .arg(p1SergeantAlive ? "🟢" : "💀");

    QString p2Text = QString(
                         "<b>Player 2</b><br>"
                         "• Scout: %1 cards %2<br>"
                         "• Sniper: %3 cards %4<br>"
                         "• Sergeant: %5 cards %6"
                         ).arg(p2Scout)
                         .arg(p2ScoutAlive ? "🟢" : "💀")
                         .arg(p2Sniper)
                         .arg(p2SniperAlive ? "🟢" : "💀")
                         .arg(p2Sergeant)
                         .arg(p2SergeantAlive ? "🟢" : "💀");

    // Count controlled tiles
    int p1Controlled = 0;
    int p2Controlled = 0;

    for (int r = 0; r < m_game->board.rows; ++r) {
        for (int c = 0; c < m_game->board.cols; ++c) {
            const Cell& cell = m_game->board.grid[r][c];
            if (cell.controlledBy == Side::A) p1Controlled++;
            else if (cell.controlledBy == Side::B) p2Controlled++;
        }
    }

    QString controlledText = QString(
                                 "<b>Controlled Tiles</b><br>"
                                 "• Player 1: %1<br>"
                                 "• Player 2: %2<br>"
                                 "<i>Win at 7 tiles</i>"
                                 ).arg(p1Controlled).arg(p2Controlled);

    player1StatsLabel->setText(p1Text);
    player2StatsLabel->setText(p2Text);
    controlledTilesLabel->setText(controlledText);
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

    // Clear previous feedback
    actionFeedbackLabel->setText("");

    if (m_selectedTile.isEmpty())
    {
        feedbackLabel->setText("⛔ Select a tile first!");
        actionFeedbackLabel->setText("Click on a cell to select it before performing an action.");
        return;
    }

    // Store target cell for later analysis
    Cell* targetCell = nullptr;
    for (int r = 0; r < m_game->board.rows; ++r) {
        for (int c = 0; c < m_game->board.cols; ++c) {
            if (m_game->board.grid[r][c].tileId.toStdString() == m_selectedTile.toStdString()) {
                targetCell = &m_game->board.grid[r][c];
                break;
            }
        }
    }

    bool success = m_game->performAction(action, m_selectedTile.toStdString());
    if (success) {
        checkForGameOver();
    }
    if (!success)
    {
        feedbackLabel->setText("❌ Action Failed");

        // Provide detailed reason based on context
        Player* current = m_game->getCurrentPlayer();
        Card* card = m_game->getCurrentCard();

        if (!card) {
            actionFeedbackLabel->setText("No card drawn. End turn and try again.");
        }
        else {
            AgentType type = card->getType();
            std::string agentName = card->getTypeName();

            if (action == "move") {
                // Get the piece that would move
                Unit* piece = current->getAgentPiece(type);
                if (!piece || !piece->getPosition()) {
                    actionFeedbackLabel->setText("Your unit is not on the board.");
                }
                else {
                    Cell* from = piece->getPosition();

                    // Check adjacency
                    bool isAdjacent = false;
                    for (Cell* neighbor : from->neighbors) {
                        if (neighbor == targetCell) {
                            isAdjacent = true;
                            break;
                        }
                    }

                    if (!isAdjacent) {
                        actionFeedbackLabel->setText("Target is not adjacent to your unit.");
                    }
                    else if (targetCell && targetCell->isOccupied()) {
                        actionFeedbackLabel->setText("Cannot move to an occupied tile.");
                    }
                    else if (type != AgentType::Scout) {
                        // Non-Scout units need marked tiles
                        if (!targetCell->marked) {
                            actionFeedbackLabel->setText("Target tile must be scouted (marked) first.");
                        }
                        else {
                            Side mySide = (current->getId() == 1 ? Side::A : Side::B);
                            if (targetCell->markedBy != mySide) {
                                actionFeedbackLabel->setText("Target tile is marked by the opponent.");
                            }
                            else {
                                actionFeedbackLabel->setText("Move failed for unknown reason.");
                            }
                        }
                    }
                    else {
                        // Scout-specific failures (shouldn't happen if adjacency and occupancy checks pass)
                        actionFeedbackLabel->setText("Move failed. Check if the tile is valid.");
                    }
                }
            }
            else if (action == "attack") {
                // Similar detailed checks can be added for attack
                actionFeedbackLabel->setText("Attack failed. Check line of sight or target not valid.");
            }
            else if (action == "scout") {
                actionFeedbackLabel->setText("Can only scout your Scout's current tile if it's not already marked.");
            }
            else if (action == "control") {
                actionFeedbackLabel->setText("Cannot control this tile. Enemy piece present or already controlled.");
            }
        }

        return;
    }

    feedbackLabel->setText("✅ Action Successful");

    // Clear selection after successful action
    if (m_selectedItem) {
        m_selectedItem->setSelected(false);
        m_selectedItem = nullptr;
    }
    m_selectedTile.clear();

    updateUI();
}

// ============================================
// Update UI
// ============================================
void GameBoardWindow::updateUI()
{
    if (!m_game) return;

    refreshHUD();
    updateStatsPanel(); // NEW: Update stats
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

    currentPlayerLabel->setText(QString::fromStdString(current->getName()));

    // Reset all buttons
    moveButton->setEnabled(false);
    attackButton->setEnabled(false);
    scoutButton->setEnabled(false);
    controlButton->setEnabled(false);

    if (m_game->getCurrentCard())
    {
        AgentType type = m_game->getCurrentCard()->getType();

        currentCardLabel->setText(
            "Current Card: " +
            QString::fromStdString(m_game->getCurrentCard()->getTypeName())
            );

        // Enable allowed actions
        switch (type)
        {
        case AgentType::Scout:
            moveButton->setEnabled(true);
            scoutButton->setEnabled(true);
            attackButton->setEnabled(true);
            break;

        case AgentType::Sniper:
            moveButton->setEnabled(true);
            attackButton->setEnabled(true);
            break;

        case AgentType::Seargeant:
            moveButton->setEnabled(true);
            attackButton->setEnabled(true);
            controlButton->setEnabled(true);
            break;

        default:
            break;
        }
    }

    deckInfoLabel->setText("Deck Size: " + QString::number(current->getDeckSize()));

    if (m_game->isGameOver()) {
        feedbackLabel->setText("🏆 GAME OVER");
    }
}

// ============================================
// Render Board From Engine
// ============================================

void GameBoardWindow::renderBoardFromGame()
{
    static int counter = 0;
    counter++;
    std::cout << "Rendering board call #" << counter << std::endl;
    if (!m_game)
        return;

    m_selectedItem = nullptr;
    m_selectedTile.clear();

    m_scene->clear();

    Board& board = m_game->board;

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
            Cell& cell = board.grid[r][c];

            if (cell.type == -1 || cell.tileId.isEmpty())
                continue;

            const qreal x =
                (horizontalOffset + tileIndex) * (cellSize + gap);

            const qreal y =
                r * (cellSize + gap);

            CellItem* item = new CellItem(&board.grid[r][c], cellSize);
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
    if (m_selectedItem && m_scene->items().contains(m_selectedItem))
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


void GameBoardWindow::checkForGameOver()
{
    if (!m_game) return;

    if (m_game->isGameOver()) {
        Player* winner = m_game->getCurrentPlayer(); // or determine winner
        QString message = QString("%1 WINS!").arg(QString::fromStdString(winner->getName()));

        QMessageBox msgBox;
        msgBox.setWindowTitle("Game Over");
        msgBox.setText(message);
        msgBox.setIcon(QMessageBox::Information);
        msgBox.setStandardButtons(QMessageBox::Ok);
        msgBox.exec();
    }
}

// beep for actions
void GameBoardWindow::playSound(bool success)
{
    if (success) {
        QApplication::beep(); // Success sound
    } else {
        // Error sound - maybe a lower beep
        std::cout << "\a"; // ASCII bell
    }
}
