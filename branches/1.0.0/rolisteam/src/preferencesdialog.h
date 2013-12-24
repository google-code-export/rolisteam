/*************************************************************************
 *     Copyright (C) 2010 by Joseph Boudou                               *
 *     Copyright (C) 2011 by Renaud Guezennec                            *
 *                                                                       *
 *     http://www.rolisteam.org/                                         *
 *                                                                       *
 *   rolisteam is free software; you can redistribute it and/or modify   *
 *   it under the terms of the GNU General Public License as published   *
 *   by the Free Software Foundation; either version 2 of the License,   *
 *   or (at your option) any later version.                              *
 *                                                                       *
 *   This program is distributed in the hope that it will be useful,     *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of      *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the       *
 *   GNU General Public License for more details.                        *
 *                                                                       *
 *   You should have received a copy of the GNU General Public License   *
 *   along with this program; if not, write to the                       *
 *   Free Software Foundation, Inc.,                                     *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.           *
 *************************************************************************/

#ifndef PREFERENCES_DIALOG_H
#define PREFERENCES_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QCheckBox>


#include "preferencesmanager.h"
#include "colorbutton.h"



#include "widgets/dirchooser.h"

namespace Ui {
class PreferencesDialogBox;
}

/**
 * @brief Actually only to change directories.
 */
class PreferencesDialog : public QDialog
{
    Q_OBJECT

public:
    PreferencesDialog(QWidget * parent = NULL, Qt::WindowFlags f = 0);
    ~PreferencesDialog();

public slots:
    void show();

private slots:
    void load();
    void save() const;
    void performDiag();

private:
#ifndef NULL_PLAYER
    DirChooser * m_gmMusicDir;
    DirChooser * m_playerMusicDir;
#endif
    DirChooser * m_picturesDir;
    DirChooser * m_mapsDir;
    DirChooser * m_sessionsDir;
    DirChooser * m_notesDir;
    DirChooser * m_chatsDir;

    PreferencesManager* m_preferences;
    QCheckBox* m_checkUpdateAtStartUp;
    ColorButton* m_fogColor;

    Ui::PreferencesDialogBox* ui;
};

#endif
