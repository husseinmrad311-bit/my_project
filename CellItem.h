#ifndef CELLITEM_H
#define CELLITEM_H

#include <QGraphicsObject>
#include <QString>
#include <QColor>
#include "Cell.h"

class CellItem : public QGraphicsObject
{
    Q_OBJECT

public:
    explicit CellItem(const Cell& cell,
                      int size = 60,
                      QGraphicsItem* parent = nullptr);

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

    QString getTileId() const;
    void setSelected(bool selected);

signals:
    void cellClicked(const QString& tileId);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;

private:
    Cell m_cell;      // FULL CELL COPY (safe)
    int  m_size;
    bool m_selected = false;
};

#endif
