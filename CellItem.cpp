#include "CellItem.h"
#include <QPainter>

CellItem::CellItem(const QString& tileId, int size)
    : m_tileId(tileId),
    m_size(size)
{
}

QRectF CellItem::boundingRect() const
{
    return QRectF(0, 0, m_size, m_size);
}

void CellItem::paint(QPainter* painter,
                     const QStyleOptionGraphicsItem*,
                     QWidget*)
{
    painter->setPen(Qt::black);
    painter->setBrush(QColor(220, 220, 220));
    painter->drawRect(boundingRect());

    // Debug: tile id only
    painter->setPen(Qt::black);
    painter->drawText(
        boundingRect(),
        Qt::AlignCenter,
        m_tileId
        );
}
