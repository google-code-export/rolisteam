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


/**
    *
    * @brief QMdiArea with backgound picture, Manging all subWindows
    *
    */


#ifndef WORKSPACE_AMELIORE_H
#define WORKSPACE_AMELIORE_H

#include <QSettings>
#include <QWidget>
#include <QImage>
#include <QPaintEvent>
#include <QMdiArea>
#include <QUrl>


#include "toolbar.h"
#include "submdiwindows.h"

class QPixmap;
class PreferencesManager;
class MainWindow;
/**
    * @brief central widget of the MainWindow, it is a QMdiArea which displays many kind of subwindows.
    */
class ImprovedWorkspace : public QMdiArea
{
    Q_OBJECT
    
public:
    /**
    * @brief Class constructor
    */
    ImprovedWorkspace(/*QColor& penColor,*/MainWindow *parent = 0);
    /**
    * @brief add subwindows in the QMdiArea. Connect subWindow to few signals.
    * @param subWindow which has to be added
    */
    void addWidget(SubMdiWindows* subWindow);
    
    /**
    * @brief set the pointer to display custom menu depending of the current window
    * @param pointer to the menu
    */
    void setVariantMenu(QMenu* menu);
    
    void setRClient(RClient* t);
    
    virtual void readSettings(QSettings & settings);
    virtual void writeSettings(QSettings & settings);
    
    SubMdiWindows* activeSubMdiWindow();
    
public slots:
    /**
    * @brief is called when the current tool changed, raise the signal currentToolHasChanged, and it performs the first treatment
    * @param the current selected tool
    */
    void currentToolChanged(ToolsBar::SelectableTool );
    /**
    * @brief is called when currentCursorChanged is raised, and it performs the first treatment
    * @param the current selected tool
    */
    void currentColorChanged(QColor&);
    
    /**
    * @brief is called when the user change the pen size
    * @param the new size
    */
    void currentPenSizeChanged(int);
    /**
    * @brief is called when the user change the PNJ size
    * @param the new size
    */
    void currentNPCSizeChanged(int);
    
signals:
    /**
    * @brief signal emitted when the current tool has changed
    * @param the current selected tool
    */
    void currentCursorChanged(QCursor*);
    /**
    * @brief signal emitted when the current tool has changed
    * @param the current selected tool
    */
    void currentToolHasChanged(ToolsBar::SelectableTool);
    
    /**
    * @brief signal emitted when the current pen color has changed
    * @param the current color
    */
    void currentColorHasChanged(QColor&);
    /**
    * @brief is called when the user change the pen size
    * @param the new size
    */
    void penSizeChanged(int);
    /**
    * @brief is called when the user change the PNJ size
    * @param the new size
    */
    void npcSizeChanged(int);
    /**
    * @brief is called when the user change the PNJ size
    * @param the new size
    */
    void currentModeChanged(int);
    
    
    void openCleverUri(CleverURI*);
    
    void openNote(QString,bool);
    
    void openPicture(QImage);
    
protected:
    /**
    * @brief Overwrite of the resize event to perform an hack to display the correct background picture
    * @param event context of the event raising
    */
    void resizeEvent ( QResizeEvent * event );
    void dropEvent ( QDropEvent * event );
    void dragEnterEvent ( QDragEnterEvent * event );
    void dragMoveEvent(QDragMoveEvent *event);
    
    
private slots:
    /**
    * @brief is called when active subwindow changed
    * @param active subwindow
    */
    void activeSubWindowChanged(QMdiSubWindow* wdw);
    
private:
    CleverURI::ContentType fileToCleverUriType(QString path, bool& ok);
    /**
    * initialize all cursor
    */
    void initCursors();
    
    /**
    * store the picture
    */
    QPixmap* m_backgroundPicture;
    
    /**
    * store the pixmap of the whole background
    */
    QPixmap* m_variableSizeBackground;
    
    /**
    * cursor for drawing
    */
    QCursor *m_drawingCursor;
    
    /**
    * cursor for text
    */
    QCursor *m_textCursor;
    /**
    * cursor for moving items
    */
    QCursor *m_moveCursor;
    /**
    * cursor for rotate items
    */
    QCursor *m_rotateCursor;
    /**
    * cursor for selecting color
    */
    QCursor *m_pipetteCursor;
    /**
    * cursor for adding items
    */
    QCursor *m_addCursor;
    /**
    * cursor for deleting items
    */
    QCursor *m_deleteCursor;
    /**
    * cursor for getting item's status.
    */
    QCursor *m_stateCursor;
    
    /**
    * current cursor.
    */
    QCursor *m_handCursor;
    
    /**
    * current cursor.
    */
    QCursor *m_currentCursor;
    
    /**
    * current pen color.
    */
    // QColor& m_currentPenColor;
    QColor m_currentPenColor;
    
    /**
    * current pen color.
    */
    QColor m_backGroundColor;
    
    MainWindow* m_parentWindow;
    
    /**
    * current pen size.
    */
    int m_penSize;
    /**
    * current npc color
    */
    int m_npcSize;
    
    /**
    * pointer to the unique instance of preference manager.
    */
    PreferencesManager* m_options;
    
    RClient* m_client;
    
    /**
    * pointer to the variant menu
    */
    QMenu* m_variantMenu;
    
    QDockWidget* m_previousWidget;
};

#endif
