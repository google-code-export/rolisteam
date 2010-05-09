/***************************************************************************
 *	Copyright (C) 2007 by Romain Campioni                                  *
 *	Copyright (C) 2009 by Renaud Guezennec                                 *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
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



#ifndef MAP_FRAME_H
#define MAP_FRAME_H

#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QAction>

#include <rgraphicsview.h>

#include "map.h"
#include "MainWindow.h"

class MapFrame : public SubMdiWindows
{
    Q_OBJECT


public :
    MapFrame(Map *map);
    ~MapFrame();
    Map *map();
    QString IdMap();

    int editingMode();
public slots :
        /**
         *  @brief called when the windows starts to move (not sure it's still used)
         *  @param the new location
         */
    void startMoving(QPoint position);

    /**
     *  @brief called when the windows is moved (not sure it's still used)
     *  @param the new location
     */
    void Moving(QPoint position);
    /**
     *  @brief change the current mice cursor
     *  @param new selected QCursor
     */
    virtual void currentCursorChanged(QCursor*);
    /**
     *  @brief change the current drawing tool
     *  @param  new selected tool
     */
    virtual void currentToolChanged(ToolsBar::SelectableTool);
    /**
     *  @brief change the current color
     *  @param  new color
     */
    virtual void currentColorChanged(QColor& );

    /**
     *  @brief change the pen size
     *  @param  new size
     */
    virtual void currentPenSizeChanged(int);
    /**
     *  @brief change the NPC size
     *  @param  new size
     */
    virtual void currentNPCSizeChanged(int);
    /**
     *  @brief change the current editing  behavior to MaskMode.
     */
    virtual void setEditingMode(int);


protected :
        /**
         *  @brief catch the closeEvent to hide itself (not delete)
         *  @param event discribe the context of the event
         */
    void closeEvent(QCloseEvent *event);

    virtual void paintEvent(QPaintEvent* event);

    virtual void mousePressEvent(QMouseEvent* event);


private :

    /**
     *  pointer to the map, the place where all items are added and displayed
     */
    Map* m_map;

    /**
     *  Used for moving the windows (not sure it's still used)
     */
    QPoint startingPoint;
    /**
     *  Used for adjusting the subWindow width (not sure it's still used)
     */
    int horizontalStart;
    /**
     *  Used for adjusting the subWindow height (not sure it's still used)
     */
    int verticalStart;

    /**
     * Pointer to the graphicView, the widget (viewport) which displays the scene
     */
    RGraphicsView* m_graphicView;


    /**
     * Pointer to the intermediate widget, it's a sublayer to manager layout.
     */
    QWidget* m_widgetLayout;

    /**
     * vertical layout
     */
    QVBoxLayout* m_vlayout;
    /**
     * horizontal layout
     */
    QHBoxLayout* m_hlayout;
    /**
     * mask pixmak
     */
    QPixmap* m_maskPixmap;
    /**
     * current edition mode
     */
    int m_currentEditingMode;
};

#endif
