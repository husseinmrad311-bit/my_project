#include "CellItem.h"
#include <QPainter>
#include <QGraphicsSceneMouseEvent>

CellItem::CellItem(const Cell& cell,
                   int size,
                   QGraphicsItem* parent)
    : QGraphicsObject(parent),
    m_cell(cell),
    m_size(size)
{
    setAcceptedMouseButtons(Qt::LeftButton);
}

QRectF CellItem::boundingRect() const
{
    return QRectF(0, 0, m_size, m_size);
}

void CellItem::paint(QPainter* painter,
                     const QStyleOptionGraphicsItem*,
                     QWidget*)
{
    // ----- Tile Color -----
    QColor color;
    switch (m_cell.type)
    {
    case 0: color = QColor(220,220,220); break;
    case 1: color = QColor(80,170,80); break;
    case 2: color = QColor(80,120,200); break;
    default: color = Qt::gray;
    }

    painter->setBrush(color);

    if (m_selected)
        painter->setPen(QPen(Qt::yellow, 3));
    else
        painter->setPen(QPen(Qt::black, 1));

    painter->drawRect(boundingRect());

    // ----- Tile ID -----
    painter->setPen(Qt::black);
    painter->drawText(boundingRect(),
                      Qt::AlignTop | Qt::AlignHCenter,
                      m_cell.tileId);

    // ----- Agent Rendering -----
    if (m_cell.agent != AgentType::None)
    {
        QColor agentColor =
            (m_cell.agentSide == Side::A)
                ? QColor(0, 0, 200)
                : QColor(200, 0, 0);

        int margin = 15;
        QRectF circleRect(margin,
                          margin,
                          m_size - 2*margin,
                          m_size - 2*margin);

        painter->setBrush(agentColor);
        painter->setPen(Qt::black);
        painter->drawEllipse(circleRect);

        QString text;
        switch (m_cell.agent)
        {
        case AgentType::Scout: text = "S"; break;
        case AgentType::Sniper: text = "N"; break;
        case AgentType::Seargeant: text = "SG"; break;
        default: break;
        }

        painter->setPen(Qt::white);
        painter->drawText(circleRect,
                          Qt::AlignCenter,
                          text);
    }
}

QString CellItem::getTileId() const
{
    return m_cell.tileId;
}

void CellItem::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

void CellItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    emit cellClicked(m_cell.tileId);
    QGraphicsObject::mousePressEvent(event);
}
