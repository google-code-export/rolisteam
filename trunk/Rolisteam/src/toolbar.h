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


#ifndef TOOLS_BAR_H
#define TOOLS_BAR_H

#include <QAction>
#include <QLineEdit>
#include <QLCDNumber>
#include <QDockWidget>
#include <QResizeEvent>

#include "colorselector.h"
#include "diameterselector.h"

/**
  *  @brief toolbar is a QDockWidget subclass which gathering all tool required for drawing maps.
  *  @todo manu members must be renamed to english
  */
class ToolsBar : public QDockWidget
{
Q_OBJECT

public :
    /**
      * @brief part of the singleton pattern
      */
    static ToolsBar* getInstance(QWidget *parent=0);
    /**
      * @brief may not be used anymore
      */
    void majCouleursPersonnelles();
    /**
      * @brief may not be used anymore
      */
    QColor donnerCouleurPersonnelle(int numero);
    /**
      * @brief may not be used anymore
      */
    void autoriserOuInterdireCouleurs();

    /**
      * @brief accessor to the current color
      */
    QColor& currentColor();
    
    // Selectable tools
    /**
      * @brief tools id
      */
    enum SelectableTool {PEN, LINE, EMPTYRECT, FILLRECT, EMPTYELLIPSE, FILLEDELLIPSE, TEXT, HANDLER, ADDNPC, DELNPC, MOVECHARACTER, STATECHARACTER};

    /**
      * @brief accessor to the current tool
      */
    ToolsBar::SelectableTool getCurrentTool();
    public slots :
    /**
      * @brief increase NPC number
      * @todo rename it
      */
    void incrementeNumeroPnj();
    /**
      * @brief accessor to set the current color
      */
    void changeCurrentColor(QColor color);
    /**
      * @brief set the size for the given NPC
      */
    void updateNPCSize(int diameter, QString name);


signals:

    /**
      * @brief emited when current tool has been changed by user
      */
    void currentToolChanged(ToolsBar::SelectableTool);
    /**
      * @brief emitted when current color has been changed by user
      */
    void currentColorChanged(QColor&);
    /**
      * @brief emitted when user has changed the pen size.
      */
    void currentPenSizeChanged(int);
    /**
      * @brief emitted when current NPC size has changed
      */
    void currentPNCSizeChanged(int);
    /**
      * @brief emitted when current mode has changed
      */
    void currentModeChanged(int);

private:
    /**
      * @brief constructor for Qt widget
      */
    ToolsBar(QWidget *parent = 0);
    
    
    static ToolsBar* m_sigleton;/// address of single instance
    void creerActions(); /// utily function
    void creerOutils(); /// utilyti function

    QWidget* m_centralWidget; /// address to the main widget
    QLineEdit *m_textEditLine; /// text line @warning is it still used ?
    QLineEdit *m_npcNameTextEdit;/// text line to define the npc name
    QLCDNumber *m_displayNPCCounter; /// count how many npc have been created
    ColorSelector *m_colorSelector; /// select a color
    DiameterSelector *m_lineDiameter;/// select pen diameter
    DiameterSelector *m_NpcDiameter; /// select npc diameter
    QActionGroup *m_toolsGroup;/// group all tools and manage which one is the current one
    SelectableTool m_currentTool; /// current tool

    QAction *m_pencilAct;
    QAction *m_lineAct;
    QAction *m_rectAct;
    QAction *m_rectFillAct;
    QAction *m_elipseAct;
    QAction *m_elipseFillAct;
    QAction *m_textAct;
    QAction *m_handAct;
    QAction *m_addPCAct;
    QAction *m_delNPCAct;
    QAction *m_movePCAct;
    QAction *m_pcStateAct;
    QAction *m_resetCountAct;
    
    /*   QAction* m_eraseAction;
    QAction* m_hideAction;
    QAction* m_unveilAction;*/
    
    
    
    QString m_currentNPCName;
    int m_currentNPCNumber;
    
    private slots :
    void resetNpcCount();
    void changeSize(bool floating);
    void changeText(const QString &texte);
    void npcNameChange(const QString &texte);
    
    
    
    void currentActionChanged(QAction* p);
    
};
    
#endif
