#ifndef CELLITEM_H
#define CELLITEM_H

#pragma once

#include <QGraphicsItem>
#include <QString>
#include <QColor>

// 🔑 Reuse game-state definitions
#include "Cell.h"

class CellItem : public QGraphicsItem
{
public:
    CellItem(const QString& tileId,
             int tileType,
             int size = 60);

    // visual setters (NO logic)
    void setMarked(bool marked);
    void setControlledBy(Side side);      // Side::A / Side::B / None
    void setAgent(AgentType agent);        // from Cell.h

    QRectF boundingRect() const override;
    void paint(QPainter* painter,
               const QStyleOptionGraphicsItem* option,
               QWidget* widget = nullptr) override;

private:
    // base info
    QString m_tileId;
    int     m_tileType;
    int     m_size;

    // visual state
    bool      m_marked  = false;
    Side      m_control = Side::None;
    AgentType m_agent   = AgentType::None;

    QColor baseColorForType() const;
};

#endif // CELLITEM_H
