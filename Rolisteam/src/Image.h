/***************************************************************************
 *	Copyright (C) 2007 by Romain Campioni   			   *
 *	Copyright (C) 2009 by Renaud Guezennec                             *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   rolisteam is free software; you can redistribute it and/or modify  *
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



#ifndef IMAGE_H
#define IMAGE_H

#include <QWidget>
#include <QImage>
#include <QScrollArea>
#include <QFile>
#include <QString>
#include <QLabel>

#include "submdiwindows.h"

class QScrollArea;
class QLabel;
class QSpinBox;
class QSlider;
class Image : public SubMdiWindows //public QScrollArea
{
    Q_OBJECT

public :
    Image(QString& m_filename, QWidget *parent = 0);
    ~Image();






public slots :
    void pointeurMain();
    void pointeurNormal();

protected :
    void closeEvent(QCloseEvent *event);



    void setUi();
   /* void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);*/

private slots:
    void setZoomLevel(int zoomlevel);
private :
    QString m_filename;
    QLabel * m_labelImage;
    QImage* m_image;

    QScrollArea* m_scrollArea;
    float m_zoomLevel;
    QPixmap  m_pixMap;
    QLabel* m_zoomLabel;
    QSlider* m_zoomSlider;
    QSpinBox* m_zoomSpinBox;



};

#endif
