#include "GameBoardWindow.h"
#include "ui_GameBoardWindow.h"
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
    ui->setupUi(this);ui->boardView->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui->boardView->setAlignment(Qt::AlignCenter);

    setWindowTitle("Undaunted - Game Board");

    m_scene = new QGraphicsScene(this);
    ui->boardView->setScene(m_scene);

    //makes the view follow resizing nicely
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

    const int cellSize = 30;   //you can change later
    const int gap = 2;

    for (int r = 0; r < rows; ++r) {
        for (int c = 0; c < cols; ++c) {
            const Cell& cell = data.board.grid[r][c];

            QRectF rect(c * (cellSize + gap),
                        r * (cellSize + gap),
                        cellSize, cellSize);

            //simple coloring by type
            QColor color;
            switch (cell.type) {
            case 0: color = QColor(220, 220, 220); break;
            case 1: color = QColor(80, 170, 80);   break;
            case 2: color = QColor(80, 120, 200);  break;
            default: color = QColor(200, 200, 80); break;
            }

            auto *item = m_scene->addRect(rect, QPen(Qt::black), QBrush(color));
            item->setToolTip(cell.tileId + " : " + QString::number(cell.type));

            //text inside cell TileID:Number
            QString cellText =  cell.tileId + ":" + QString::number(cell.type) ;

            auto *textItem = m_scene->addText(cellText);
            QFont f = textItem->font();
            f.setBold(true);
            f.setPointSize(10);              //we can change this
            textItem->setFont(f);

            //center text inside the rectangle
            QRectF tb = textItem->boundingRect();
            textItem->setPos(
                rect.center().x() - tb.width() / 2.0,
                rect.center().y() - tb.height() / 2.0
                );

            // make text readable on dark colors
            textItem->setDefaultTextColor(Qt::black);
            if (cell.type == 2) {            //blue cell
                textItem->setDefaultTextColor(Qt::white);
            }

        }
    }

    //fit all drawn items inside the view
    ui->boardView->fitInView(m_scene->itemsBoundingRect(), Qt::KeepAspectRatio);
}

void GameBoardWindow::fitBoardToView()
{
    if (!ui->boardView || !ui->boardView->scene())
        return;

    QRectF r = ui->boardView->scene()->itemsBoundingRect();
    if (r.isNull())
        return;

    ui->boardView->setSceneRect(r);


    ui->boardView->resetTransform();                //this fixes most "still tiny" cases
    ui->boardView->fitInView(r, Qt::KeepAspectRatio);
}


void GameBoardWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    fitBoardToView();
}

