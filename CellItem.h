#ifndef CELLITEM_H
#define CELLITEM_H

#pragma once

#include <QGraphicsItem>
#include <QString>

class CellItem : public QGraphicsItem
{
public:
    explicit CellItem(const QString& tileId,
                      int size = 60);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

private:
    QString m_tileId;
    int     m_size;
};

#endif // CELLITEM_H
