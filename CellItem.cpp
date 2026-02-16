#include "CellItem.h"

CellItem::CellItem(Cell* cell,
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
    if (!m_cell)
        return;

    painter->setRenderHint(QPainter::Antialiasing, true);

    // --------------------------------------------------
    // Base Terrain Color
    // --------------------------------------------------
    QColor color;

    switch (m_cell->type)
    {
    case 0: color = QColor(220,220,220); break; // neutral
    case 1: color = QColor(80,170,80);   break; // forest
    case 2: color = QColor(80,120,200);  break; // house
    default: color = Qt::gray;
    }

    // --------------------------------------------------
    // Controlled Tile Overlay
    // --------------------------------------------------
    if (m_cell->controlledBy == Side::A)
        color = QColor(120,170,255); // Blue tint
    else if (m_cell->controlledBy == Side::B)
        color = QColor(255,140,140); // Red tint

    painter->setBrush(color);

    // --------------------------------------------------
    // Border
    // --------------------------------------------------
    QPen borderPen = m_selected
                         ? QPen(Qt::yellow, 3)
                         : QPen(Qt::black, 1);

    painter->setPen(borderPen);
    painter->drawRect(boundingRect());

    // --------------------------------------------------
    // Tile ID
    // --------------------------------------------------
    painter->setPen(Qt::black);
    painter->drawText(boundingRect(),
                      Qt::AlignTop | Qt::AlignHCenter,
                      m_cell->tileId);

    // --------------------------------------------------
    // Agent Rendering
    // --------------------------------------------------
    if (m_cell->agent != AgentType::None)
    {
        QColor agentColor =
            (m_cell->agentSide == Side::A)
                ? QColor(0,0,200)     // Player A
                : QColor(200,0,0);    // Player B

        int margin = 15;
        QRectF circleRect(margin,
                          margin,
                          m_size - 2*margin,
                          m_size - 2*margin);

        painter->setBrush(agentColor);
        painter->setPen(Qt::black);
        painter->drawEllipse(circleRect);

        QString label;
        switch (m_cell->agent)
        {
        case AgentType::Scout:     label = "S";  break;
        case AgentType::Sniper:    label = "N";  break;
        case AgentType::Seargeant: label = "SG"; break;
        default: break;
        }

        painter->setPen(Qt::white);
        painter->drawText(circleRect,
                          Qt::AlignCenter,
                          label);
    }

    // --------------------------------------------------
    // Mark Indicator (Dashed Border)
    // --------------------------------------------------
    if (m_cell->marked)
    {
        QPen markPen(
            (m_cell->markedBy == Side::A)
                ? QColor(0,255,0)      // Green
                : QColor(0,255,255)    // Cyan
            );

        markPen.setStyle(Qt::DashLine);
        markPen.setWidth(2);

        painter->setPen(markPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawRect(boundingRect().adjusted(4,4,-4,-4));
    }
}

QString CellItem::getTileId() const
{
    return m_cell ? m_cell->tileId : "";
}

void CellItem::setSelected(bool selected)
{
    m_selected = selected;
    update();
}

void CellItem::mousePressEvent(QGraphicsSceneMouseEvent* event)
{
    if (m_cell)
        emit cellClicked(m_cell->tileId);

    QGraphicsObject::mousePressEvent(event);
}
