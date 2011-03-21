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




#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include "ToolBar.h"


#include "tchat.h"
#include "Image.h"
#include "MinutesEditor.h"
#include "audioplayer.h"
#include "userlistwidget.h"
#include "mapwizzarddialog.h"


#include "connection.h"
#include "cleveruri.h"

class PreferencesManager;
class ImprovedWorkspace;
class MapFrame;
class Carte;
class QActionGroup;
class PreferenceDialog;
class ConnectionWizzard;
class CharacterSheetWindow;
class Player;
class DicePlugInManager;
class UpdateChecker;
class SessionManager;
class Session;





/**
  * @brief stores file URIs and keeps its type, useful for recent opened files
  */
#include "rclient.h"

/**
  * @brief is the main GUI of rolisteam, bring together all piece of software components, read the settings, set the graphical interface.
  */
class MainWindow : public QMainWindow
{
Q_OBJECT

public :
        /**
          * @brief default constructor
          */
        MainWindow();
        /**
          * @brief destructor
          */
        ~MainWindow();
        /**
          * @brief allows to know if the given widget is the active Window
          * @param widget (subwindows)
          * @return true when given widget is the active one, otherwise false.
          */
        bool isActiveWindow(QWidget *widget);
public slots :     
        /**
          * @brief
          */
        void displayMinutesEditor();
        void addCharacterSheet();

        void checkUpdate();


protected :
        void closeEvent(QCloseEvent *event);

private :

        /**
         * @brief determine either the current content should be save or not
         */
        bool maybeSave();
        /**
         * @brief create menus.
         */
        void createMenu();
        /**
         * @brief Connect actions to appropriate slots
         */
        void connectActions();
        /**
         * @brief empty
         */
        void allowActions();
        /**
         * @brief Save All contents
         */
        void saveAll();
        /**
         * @brief perform some action before adding the given subwindows into the QMdiArea
         */
        void addToWorkspace(SubMdiWindows* subWindow);

        /**
         * @brief Load informations from the previous rolisteam's execution
         */
        void readSettings();

        /**
         * @brief Save parameters for next executions.
         */
        void writeSettings();
        /**
         * @brief Save parameters for next executions.
         */
        ImprovedWorkspace* m_workspace;

        /**
         * @brief Save parameters for next executions.
         */
       // ToolsBar *m_toolbar;
        /**
          * @brief unique acces to recentfile management
          */
        void addopenedFile(QString& , CleverURI::ContentType );




        /// members declarations.
        AudioPlayer* m_audioPlayer;

        QMenu *m_fileMenu;
        QMenu* m_newMenu;
        QAction* m_newMapAct;
        QAction* m_newNoteAct;
        QAction* m_newScenarioAct;

        QMenu* m_openMenu;
        QAction* m_openMapAct;
        QAction* m_openPictureAct;
        QAction* m_openScenarioAct;
        QAction* m_openCharacterSheetsAct;
        QAction* m_openNoteAct;

        QMenu* m_recentFilesMenu;

        QAction* m_saveAct;
        QAction* m_saveAsAct;
        QAction* m_saveAllAct;
        QAction* m_saveAllIntoScenarioAct;

        QAction *m_preferencesAct;

        QAction* m_closeAct;
        QAction* m_quitAct;

        QMenu *m_viewMenu;

        QAction* m_usedTabBarAct;
        QMenu* m_organizeMenu;
        QAction* m_cascadeSubWindowsAct;
        QAction* m_tileSubWindowsAct;
        QAction* m_noteEditoAct;
        QAction* m_dataSheetAct;


        //Help menu
        QMenu *m_helpMenu;
        QAction *m_helpAct;
        QAction *m_aproposAct;


        /**
          * pointer to the unique instance of preference manager.
          */
        PreferencesManager* m_options;

        /**
          * pointer to the unique instance of dice plugin manager.
          */
        DicePlugInManager* m_diceManager;

        /**
          * pointer to the userlist widget.
          */
        UserListWidget* m_playerListWidget;
        /**
          *
          */
        Player* m_player;

        QMenu* m_currentWindowMenu;



        /**
          * pointer to the network menu
          */
        QMenu* m_networkMenu;
        /**
          * action to start the server in the localhost
          */
        QAction* m_serverAct;
        /**
          * action to display new connection dialog
          */
        QAction* m_newConnectionAct;

        /**
          * action to display the connection manager
          */
        QAction* m_manageConnectionAct;

        QActionGroup* m_connectionActGroup;


        QActionGroup* m_subWindowActGroup;
        QMap<QAction*,SubMdiWindows*>* m_subWindowList;
        QMap<QAction*,Connection>* m_connectionMap;
        //QList<QAction*> m_registedConnectionList;

        /**
          * List of connections
          */
        ConnectionList m_connectionList;

        Connection m_currentConnection;
        RClient* m_rclient;
        /**
          * preference wizzard.
          */
        PreferenceDialog* m_preferenceDialog;

        /**
          * preference wizzard.
          */
        ConnectionWizzard* m_connectDialog;

        QActionGroup* m_recentFilesActGroup;
        /**
          * @brief QStringList of opened documents : file paths
          */

      /*  QStringList m_recentFiles;
        QList<int> m_recentTypes;*/
        //QList<CleverURI> m_recentFiles;
        Session* m_session;

        UpdateChecker* m_updateChecker;

        SessionManager* m_sessionManager;
private slots :
        /**
        * @brief Show the map wizzard
        *
        */
        void clickOnMapWizzard();

        /**
        * @brief Show the map wizzard
        *
        */
        void openImage(QString filepath);
        void tcpStateConnectionChanged(RClient::State);
        void askOpenImage();
        /**
        * @brief Show the about dialog
        *
        */
        void about();
        /**
        * @brief click on selected connection
        *
        */
        void onConnection(QAction*);

        /// @brief open the Qt assistant with the rolisteam documentation
        void help();


        void hideShowWindow(QAction*);


        /**
         * @brief is called when user click on start server item menu.
         */
        void startServer();
        /**
         * @brief is called when user click on add connection item menu.
         */
        void addConnection();
        /**
         * @brief is called when user click on connection manager item menu.
         */
        void showConnectionManager();

        /**
         * @brief is called when user click on preference item menu.
         */
        void showPreferenceManager();

        /**
         * @brief is called when user click on usedTabBarAct item menu.
         */
        void onTabBar();

        void openRecentFile(QAction*);
        void AskCharacterSheets();

        void openCharacterSheets(QString);

        void openTchat();

        void updateMayBeNeeded();

};

#endif
