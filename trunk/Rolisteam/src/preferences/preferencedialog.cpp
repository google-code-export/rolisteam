/***************************************************************************
 *	 Copyright (C) 2009 by Renaud Guezennec                                *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
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

#include "preferencedialog.h"
#include "ui_preferencedialog.h"
#include "preferencesmanager.h"
#include "theme.h"
#include <QFileDialog>
#include "themelistmodel.h"
#include <QDebug>
PreferenceDialog::PreferenceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PreferenceDialog)
{
    ui->setupUi(this);
    m_options = PreferencesManager::getInstance();

    m_listModel = new ThemeListModel();

    //init value:
   // initValues();
    addDefaultTheme();

    connect(ui->m_wsBgBrowserButton,SIGNAL(clicked()),this,SLOT(changeBackgroundImage()));
    connect(ui->m_wsBgColorButton,SIGNAL(colorChanged(QColor)),this,SLOT(modifiedSettings()));
    connect(ui->m_addTheme,SIGNAL(clicked()),this,SLOT(addTheme()));
    connect(ui->m_removeTheme,SIGNAL(clicked()),this,SLOT(removeSelectedTheme()));

    ui->m_themeList->setModel(m_listModel);

    connect(ui->m_themeList,SIGNAL(pressed(QModelIndex)),this,SLOT(currentChanged()));

    connect(ui->m_buttonbox,SIGNAL(clicked(QAbstractButton * )),this,SLOT(applyAllChanges(QAbstractButton * )));

}

PreferenceDialog::~PreferenceDialog()
{
    delete ui;
}

void PreferenceDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void PreferenceDialog::initValues()
{
     qDebug() << "Init Value";
    // Look and feel panel
    ui->m_wsBgPathLine->setText(m_options->value("worspace/background/image",":/resources/icons/fond workspace macos.bmp").toString());
    ui->m_wsBgColorButton->setColor(m_options->value("worspace/background/color",QColor(191,191,191)).value<QColor>());



    //General Panel
    ui->m_genUpdateCheck->setChecked(m_options->value("mainwindow/network/checkupdate",true).toBool());

    ui->m_dataChooser->setPath(m_options->value("DataDirectory",QDir::homePath()).toString());
    ui->m_dataChooser->activeDirectory(true);

    ui->m_imageChooser->setPath(m_options->value("ImageDirectory",QDir::homePath()).toString());
    ui->m_imageChooser->activeDirectory(true);

    ui->m_mapsChooser->setPath(m_options->value("MapDirectory",QDir::homePath()).toString());
    ui->m_mapsChooser->activeDirectory(true);

    ui->m_scriptChooser->setPath(m_options->value("ScriptDirectory",QDir::homePath()).toString());
    ui->m_scriptChooser->activeDirectory(true);

    ui->m_minutesChooser->setPath(m_options->value("MinutesDirectory",QDir::homePath()).toString());
    ui->m_minutesChooser->activeDirectory(true);

    ui->m_chatChooser->setPath(m_options->value("TchatDirectory",QDir::homePath()).toString());
    ui->m_chatChooser->activeDirectory(true);

    ui->m_musicChooser->setPath(m_options->value("MusicDirectory",QDir::homePath()).toString());
    ui->m_musicChooser->activeDirectory(true);

}
void PreferenceDialog::resetValues()
{
    ui->m_wsBgPathLine->setText(":/resources/icons/fond workspace macos.bmp");
    ui->m_wsBgColorButton->setColor(QColor(191,191,191));
    ui->m_genUpdateCheck->setChecked(true);

    ui->m_dataChooser->setPath(QDir::homePath());
    ui->m_scriptChooser->setPath(QDir::homePath());
    ui->m_mapsChooser->setPath(QDir::homePath());
    ui->m_minutesChooser->setPath(QDir::homePath());
    ui->m_imageChooser->setPath(QDir::homePath());
    ui->m_chatChooser->setPath(QDir::homePath());
    ui->m_musicChooser->setPath(QDir::homePath());
}
void PreferenceDialog::changeBackgroundImage()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Rolisteam Background"),".",tr(
        "Supported files (*.jpg *.png *.gif *.svg)"));

    if(!fileName.isEmpty())
    {
        ui->m_wsBgPathLine->setText(fileName);
        m_current.setBackgroundImage(fileName);
        modifiedSettings();
    }
}
void PreferenceDialog::modifiedSettings()
{
    setWindowModified(true);
}
void PreferenceDialog::applyAllChanges(QAbstractButton * button)
{
    if(QDialogButtonBox::ApplyRole==ui->m_buttonbox->buttonRole(button))
    {
        m_current.setBackgroundImage(ui->m_wsBgPathLine->text());
        m_options->registerValue("worspace/background/image",m_current.backgroundImage());
        m_options->registerValue("worspace/background/color",m_current.backgroundColor());
        m_options->registerValue("mainwindow/network/checkupdate",ui->m_genUpdateCheck->isChecked());



        m_options->registerValue("DataDirectory",ui->m_dataChooser->getPath());
        m_options->registerValue("ImageDirectory",ui->m_dataChooser->getPath());
        m_options->registerValue("MapDirectory",ui->m_mapsChooser->getPath());
        m_options->registerValue("ScriptDirectory",ui->m_scriptChooser->getPath());
        m_options->registerValue("MinutesDirectory",ui->m_minutesChooser->getPath());
        m_options->registerValue("TchatDirectory",ui->m_chatChooser->getPath());
        m_options->registerValue("MusicDirectory",ui->m_musicChooser->getPath());






        emit preferencesChanged();
    }
    setWindowModified(false);
}
void PreferenceDialog::addDefaultTheme()
{
    m_current.setName(tr("Default"));
    m_current.setBackgroundColor(m_options->value("worspace/background/color",QColor(191,191,191)).value<QColor>());
    m_current.setBackgroundImage(m_options->value("worspace/background/image",":/resources/icones/fond workspace macos.bmp").toString());
    m_listModel->addTheme(m_current);
}

void PreferenceDialog::addTheme()
{
    Theme tmp;
    tmp.setName(tr("Theme %1").arg(m_listModel->rowCount(QModelIndex())));
    m_listModel->addTheme(tmp);
}
void PreferenceDialog::removeSelectedTheme()
{
    m_listModel->removeSelectedTheme(ui->m_themeList->currentIndex().row());
}
void PreferenceDialog::currentChanged()
{
    m_current=m_listModel->getTheme(ui->m_themeList->currentIndex().row());
    refreshDialogWidgets();
}
void PreferenceDialog::refreshDialogWidgets()
{
     ui->m_wsBgPathLine->setText(m_current.backgroundImage());
     ui->m_wsBgColorButton->setColor(m_current.backgroundColor());
}

void PreferenceDialog::readSettings()
{

    qDebug() << "read setting in preference dialo";
    m_listModel->readSettings();
}
void PreferenceDialog::writeSettings()
{
    qDebug() << "write setting in preference dialog";
    m_listModel->writeSettings();
}
