#ifndef GAMEBOARDWINDOW_H
#define GAMEBOARDWINDOW_H

#include <QWidget>
#include <QGraphicsScene>
#include <QResizeEvent>
#include <QString>

#include "MapLoader.h"   //Map data

QT_BEGIN_NAMESPACE
namespace Ui { class GameBoardWindow; }
QT_END_NAMESPACE

class GameBoardWindow : public QWidget
{
    Q_OBJECT

public:
    explicit GameBoardWindow(QWidget *parent = nullptr);
    ~GameBoardWindow();

    void setPlayerNames(const QString& p1, const QString& p2);
    bool loadAndShowMap(const QString& mapPath, QString& error);

private:
    Ui::GameBoardWindow *ui;

    QGraphicsScene *m_scene = nullptr;

    //keep the loaded map so resizeEvent can refit correctly
    MapData m_currentMap;
    bool m_hasMap = false;

    void fitBoardToView();
    void renderBoard(const MapData& data);

protected:
    void resizeEvent(QResizeEvent *event) override;
};

#endif
