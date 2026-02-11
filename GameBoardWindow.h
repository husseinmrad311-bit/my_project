#ifndef GAMEBOARDWINDOW_H
#define GAMEBOARDWINDOW_H

#include <QWidget>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QLabel>
#include <QPushButton>
#include <QResizeEvent>

#include "Game.h"

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

private:
    Game* m_game = nullptr;
    QGraphicsScene* m_scene = nullptr;

    // ===== HUD =====
    QLabel* turnLabel = nullptr;
    QLabel* currentPlayerLabel = nullptr;
    QLabel* currentCardLabel = nullptr;
    QLabel* feedbackLabel = nullptr;
    QLabel* deckInfoLabel = nullptr;

    // ===== Buttons =====
    QPushButton* moveButton = nullptr;
    QPushButton* attackButton = nullptr;
    QPushButton* scoutButton = nullptr;
    QPushButton* controlButton = nullptr;
    QPushButton* endTurnButton = nullptr;

    // ===== Board =====
    QGraphicsView* boardView = nullptr;
};

#endif
