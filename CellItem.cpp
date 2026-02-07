#include "CellItem.h"
#include <QPainter>

CellItem::CellItem(const QString& tileId, int tileType, int size)
    : m_tileId(tileId),
    m_tileType(tileType),
    m_size(size)
{
    setFlag(QGraphicsItem::ItemUsesExtendedStyleOption);
}

QRectF CellItem::boundingRect() const
{
    return QRectF(0, 0, m_size, m_size);
}

void CellItem::setMarked(bool marked)
{
    m_marked = marked;
    update();
}

void CellItem::setControlledBy(Side side)
{
    m_control = side;
    update();
}

void CellItem::setAgent(AgentType agent)
{
    m_agent = agent;
    update();
}

QColor CellItem::baseColorForType() const
{
    switch (m_tileType) {
    case 0: return QColor(220, 220, 220);
    case 1: return QColor(80, 170, 80);
    case 2: return QColor(80, 120, 200);
    default: return QColor(200, 200, 80);
    }
}

void CellItem::paint(QPainter* painter,
                     const QStyleOptionGraphicsItem*,
                     QWidget*)
{
    painter->setRenderHint(QPainter::Antialiasing, true);

    // 1️⃣ base tile
    painter->setPen(Qt::black);
    painter->setBrush(baseColorForType());
    painter->drawRect(boundingRect());

    // 2️⃣ control overlay (Side-based)
    if (m_control != Side::None) {
        QColor c;
        if (m_control == Side::A) {
            c = QColor(255, 0, 0, 80);   // Player A → red
        } else if (m_control == Side::B) {
            c = QColor(0, 0, 255, 80);   // Player B → blue
        }

        painter->setBrush(c);
        painter->setPen(Qt::NoPen);
        painter->drawRect(boundingRect());
    }

    // 3️⃣ mark overlay
    if (m_marked) {
        painter->setPen(QPen(Qt::yellow, 3));
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(boundingRect().adjusted(6, 6, -6, -6));
    }

    // 4️⃣ agent icon (placeholder)
    if (m_agent != AgentType::None) {
        painter->setPen(Qt::NoPen);
        painter->setBrush(Qt::black);
        painter->drawEllipse(
            boundingRect().center(),
            m_size / 8,
            m_size / 8
            );
    }

    // 5️⃣ tile id text
    painter->setPen(Qt::black);
    painter->drawText(
        boundingRect(),
        Qt::AlignCenter,
        m_tileId
        );
}
