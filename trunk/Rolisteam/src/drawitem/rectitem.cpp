/***************************************************************************
  *      Copyright (C) 2010 by Renaud Guezennec                             *
 *                                                                         *
 *                                                                         *
 *   rolisteam is free software; you can redistribute it and/or modify     *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#include "rectitem.h"

#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>
#include <QDebug>
RectItem::RectItem(QPointF& topleft,QPointF& buttomright,bool filled,QColor& penColor,QGraphicsItem * parent)
    : VisualItem(penColor,parent)
{

    m_rect.setBottomRight(buttomright);
    m_rect.setTopLeft(topleft);
     m_filled= filled;
}

QRectF RectItem::boundingRect() const
{
    return m_rect;
}
void RectItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    if(!m_filled)
    {
        painter->setPen(m_color);
        painter->drawRect(m_rect);
    }
    else
    {
        painter->setBrush(QBrush(m_color));
        painter->fillRect(m_rect, m_color);

    }
    if(option->state & QStyle::State_Selected)
    {
        painter->save();
        painter->setBrush(Qt::NoBrush);
        QRectF tmp_rect=m_rect.adjusted(5,5,-5,-5);
        QPen pen=painter->pen();
        pen.setWidth(2);
        pen.setStyle(Qt::SolidLine);
        pen.setColor(Qt::white);
        painter->setPen(pen);
        painter->drawRect(tmp_rect);
        pen.setColor(Qt::black);
        QVector<qreal> dashes;
        dashes << tmp_rect.width()/20 << tmp_rect.width()/20;
        pen.setDashPattern(dashes);

        painter->setPen(pen);
        painter->drawRect(tmp_rect);
        painter->restore();
    }
    painter->restore();

}
void RectItem::setNewEnd(QPointF& p)
{
    m_rect.setBottomRight(p);
}
void RectItem::writeData(QDataStream& out) const
{
    out << m_rect;
    out << m_filled;
}

void RectItem::readData(QDataStream& in)
{
    in >> m_rect;
    in >> m_filled;
}
