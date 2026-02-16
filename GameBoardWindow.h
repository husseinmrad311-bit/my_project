#ifndef GAMEBOARDWINDOW_H
#define GAMEBOARDWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>
#include "CellItem.h"
#include "Game.h"
#include <QMessageBox>

class GameBoardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoardWindow(QWidget *parent = nullptr);

    void setGame(Game* game);
    void initializeGame(const QString& p1, const QString& p2);

protected:
    void resizeEvent(QResizeEvent *event) override;

private:
    void buildUI();
    void updateUI();
    void refreshHUD();
    void renderBoardFromGame();
    void fitBoardToView();
    void handleAction(const std::string& action);
    void updateStatsPanel(); // Update player stats

    void playSound(bool success) ;
    Game* m_game = nullptr;
    QGraphicsScene* m_scene = nullptr;

    //HUD
    QLabel* turnLabel = nullptr;
    QLabel* currentPlayerLabel = nullptr;
    QLabel* currentCardLabel = nullptr;
    QLabel* feedbackLabel = nullptr;
    QLabel* deckInfoLabel = nullptr;

    // ===== Stats Panel Labels =====
    QLabel* statsTitleLabel = nullptr;
    QLabel* player1StatsLabel = nullptr;
    QLabel* player2StatsLabel = nullptr;
    QLabel* controlledTilesLabel = nullptr;
    // ===== detailed Feedback =====
    QLabel* actionFeedbackLabel = nullptr;
    QString lastActionMessage;

    // buttons
    QPushButton* moveButton = nullptr;
    QPushButton* attackButton = nullptr;
    QPushButton* scoutButton = nullptr;
    QPushButton* controlButton = nullptr;
    QPushButton* endTurnButton = nullptr;

    //board
    QGraphicsView* boardView = nullptr;
    QString m_selectedTile;
    CellItem* m_selectedItem = nullptr;

private slots:
    void onCellClicked(const QString& tileId);
    void checkForGameOver() ;
};

#endif
