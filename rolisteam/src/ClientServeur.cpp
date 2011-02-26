/************************************************************************
*   Copyright (C) 2007 by Romain Campioni                               *
*   Copyright (C) 2009 by Renaud Guezennec                              *
*   Copyright (C) 2010 by Joseph Boudou                                 *
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


#include "ClientServeur.h"

#include <QTcpSocket>
#include <QMessageBox>

#include "connectiondialog.h"
#include "initialisation.h"
#include "Liaison.h"
#include "MainWindow.h"
#include "persons.h"
#include "playersList.h"

#include "variablesGlobales.h"


/********************************************************************/
/* Variables globales utilisees par tous les elements de            */
/* l'application                                                    */
/********************************************************************/    

// True si l'utilisateur est un joueur, false s'il est MJ
bool G_joueur;
// True si l'ordinateur local est client, false s'il est serveur
bool G_client;


/********************
 * Global functions *
 ********************/

void emettre(char *donnees, quint32 taille, Liaison *sauf)
{
    G_clientServeur->emettreDonnees(donnees, taille, sauf);
}


/********************************
 * Private non-member functions *
 ********************************/

static void synchronizeInitialisation(const ConnectionConfigDialog & dialog)
{
    G_initialisation.nomUtilisateur     = dialog.getName();
    G_initialisation.couleurUtilisateur = dialog.getColor();
    G_initialisation.joueur             = G_joueur             = !dialog.isGM();
    G_initialisation.client             = G_client             = !dialog.isServer();
    G_initialisation.ipServeur          = dialog.getHost();
    G_initialisation.portServeur        = dialog.getPort();
    G_initialisation.portClient.setNum(G_initialisation.portServeur);
}


/*****************
 * ClientServeur *
 *****************/

ClientServeur::ClientServeur()
: QObject(), m_server(NULL)
{
    connect(this, SIGNAL(linkAdded(Liaison *)),
            &g_featuresList, SLOT(sendThemAll(Liaison *)));
}


ClientServeur::~ClientServeur()
{
    // QObject should delete all for us.
}

bool ClientServeur::configAndConnect()
{
    ConnectionConfigDialog configDialog(
        G_initialisation.nomUtilisateur, G_initialisation.couleurUtilisateur, !G_initialisation.joueur,
        G_initialisation.ipServeur, G_initialisation.portServeur, !G_initialisation.client
    );

    ConnectionWaitDialog waitDialog;
    QMessageBox errorDialog(QMessageBox::Warning, tr("Erreur"), tr("Impossible de se connecter"));
    QTcpSocket * socket;

    PlayersList & g_playersList = PlayersList::instance();

    bool cont = true;
    while (cont)
    {
        // If the user abort configDialog, we quit
        if (configDialog.exec() == QDialog::Rejected)
            return false;

        // Server setup
        if (configDialog.isServer())
        {
            if (m_server == NULL)
            {
                m_server = new QTcpServer(this);
                connect(m_server, SIGNAL(newConnection()), this, SLOT(nouveauClientConnecte()));
            }

            // Listen successed
            if (m_server->listen(QHostAddress::Any, configDialog.getPort()))
            {
                synchronizeInitialisation(configDialog);
                connect(this, SIGNAL(linkDeleted(Liaison *)), &g_playersList, SLOT(delPlayerWithLink(Liaison *)));
                cont = false;
            }

            // Listen failed
            else
            {
                errorDialog.setInformativeText(m_server->errorString());
                errorDialog.exec();
            }
        }

        // Client setup
        else
        {
            socket = waitDialog.connectTo(configDialog.getHost(), configDialog.getPort());

            // connect successed
            if (socket != NULL)
            {
                synchronizeInitialisation(configDialog);
                new Liaison(socket);
                cont = false;
            }
            else
            {
                errorDialog.setInformativeText(waitDialog.getError());
                errorDialog.exec();
            }
        }
    }

    Player * localPlayer = new Player(
            QUuid(G_idJoueurLocal),
            configDialog.getName(),
            configDialog.getColor(),
            configDialog.isGM()
        );
    g_playersList.setLocalPlayer(localPlayer);

    return true;
}


void ClientServeur::emettreDonnees(char *donnees, quint32 taille, Liaison *sauf)
{
    // Demande d'emission vers toutes les liaisons
    emit emissionDonnees(donnees, taille, sauf);
}


void ClientServeur::ajouterLiaison(Liaison *liaison)
{
    liaisons.append(liaison);
    emit linkAdded(liaison);
}


void ClientServeur::nouveauClientConnecte()
{
    // Recuperation du socket lie a la demande de connexion
    QTcpSocket *socketTcp = m_server->nextPendingConnection();

    // Creation de la liaison qui va gerer le socket
    new Liaison(socketTcp);
}


void ClientServeur::finDeLiaison(Liaison * link)
{
    link->deleteLater();

    emit linkDeleted(link);

    // Si l'ordinateur local est un client
    if (G_client)
    {
        // On quitte l'application    
        G_mainWindow->quitterApplication(true);
    }

    // Si l'ordinateur local est le serveur
    else
    {
        if (link == NULL)
        {
            qWarning("NULL Liaison pointer (ClientServeur::finDeLiaison).");
            return;
        }

        int i = liaisons.indexOf(link);
        if (i < 0)
        {
            qWarning("Un thread inconnu vient de se terminer (finDeLiaison - ClientServeur.cpp)");
            return;
        }
        
        // On supprime la liaison de la liste, apres l'avoir detruite
        liaisons.removeAt(i);
    }
}
