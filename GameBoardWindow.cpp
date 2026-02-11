#include "GameBoardWindow.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsRectItem>
#include <QPen>
#include <QBrush>
#include <QPainter>

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
    mainLayout->addWidget(boardView, 1);
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

    bool success = m_game->performAction(action, "");

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
    if (!m_game) return;

    m_scene->clear();

    const Board& board = m_game->getBoard();

    const int cellSize = 60;
    const int gap = 8;

    for (int r = 0; r < board.rows; ++r)
    {
        for (int c = 0; c < board.cols; ++c)
        {
            const Cell& cell = board.grid[r][c];

            if (cell.type == -1 || cell.tileId.isEmpty())
                continue;

            qreal x = c * (cellSize + gap);
            qreal y = r * (cellSize + gap);

            QRectF rect(x, y, cellSize, cellSize);

            QColor baseColor;

            switch (cell.type)
            {
            case 0: baseColor = QColor(220,220,220); break;
            case 1: baseColor = QColor(80,170,80); break;
            case 2: baseColor = QColor(80,120,200); break;
            default: baseColor = Qt::gray;
            }

            m_scene->addRect(rect, QPen(Qt::black), QBrush(baseColor));

            // Draw agent if exists
            if (cell.agent != AgentType::None)
            {
                QColor pieceColor =
                    (cell.agentSide == Side::A)
                        ? Qt::red
                        : Qt::blue;

                QRectF inner = rect.adjusted(15,15,-15,-15);
                m_scene->addEllipse(inner,
                                    QPen(Qt::black),
                                    QBrush(pieceColor));
            }

            // Tile label
            auto textItem = m_scene->addText(cell.tileId);
            QRectF tb = textItem->boundingRect();
            textItem->setPos(
                rect.center().x() - tb.width()/2,
                rect.center().y() - tb.height()/2
                );
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
