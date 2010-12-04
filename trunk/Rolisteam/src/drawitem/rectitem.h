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
#ifndef RECTITEM_H
#define RECTITEM_H

#include "visualitem.h"
/**
  * @brief displays and manages rectangle on map, part of QGraphicsScene/view.
  * @todo add features for amend size, square, rotation, make it bigger/smaller, move, selection....
  */
class RectItem : public VisualItem
{
public:
    RectItem(QPointF& topleft,QPointF& buttomright,bool filled,QColor& penColor,QGraphicsItem * parent = 0);



    void paint ( QPainter * painter, const QStyleOptionGraphicsItem * option, QWidget * widget = 0 );

    virtual QRectF boundingRect() const ;

    virtual void setNewEnd(QPointF& nend);
private:
    QRectF m_rect;

    bool m_filled;


};

#endif // RECTITEM_H
