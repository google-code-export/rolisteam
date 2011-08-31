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
#include "lineitem.h"
#include <QPainterPath>
#include <QPainter>

LineItem::LineItem()
    : VisualItem()
{

}

LineItem::LineItem(QPointF& p,QColor& penColor,int penSize,QGraphicsItem * parent)
    :  VisualItem(penColor,parent)
{
    m_startPoint = p;
    m_rect.setTopLeft(p);
    m_pen.setColor(penColor);
    m_pen.setWidth(penSize);
}

void LineItem::setNewEnd(QPointF& nend)
{
    m_endPoint = nend;
    m_rect.setBottomRight(nend);
}
QRectF LineItem::boundingRect() const
{
    return  m_rect;
}
void LineItem::paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget)
{
    painter->save();
    painter->setPen(m_pen);
    painter->drawLine(m_startPoint,m_endPoint);
    painter->restore();

}
void LineItem::writeData(QDataStream& out) const
{
    out << m_rect;
    out << m_startPoint;
    out << m_endPoint;
    out << m_pen;
    out << m_filled;
    out << m_color;
}

void LineItem::readData(QDataStream& in)
{
    in >> m_rect;
    in >> m_startPoint;
    in >> m_endPoint;
    in >> m_pen;
    in >> m_filled;
    in >> m_color;
}
VisualItem::ItemType LineItem::getType()
{
    return VisualItem::LINE;
}
