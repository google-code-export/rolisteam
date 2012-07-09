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

#define second 1000
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
    Initialisation* init = Initialisation::getInstance();
    init->setUserName(dialog.getName());
    init->setUserColor(dialog.getColor());
    init->setPlayer(!dialog.isGM());
    init->setClient(!dialog.isServer());

    G_joueur= !dialog.isGM();
    G_client= !dialog.isServer();
    init->setIpAddress(dialog.getHost());
    init->setServerPort(dialog.getPort());
    init->setClientPort(QString().setNum(dialog.getPort()));

}
/*****************
 * TimerDialog *
 *****************/
TimerDialog::TimerDialog(int interval,QString message, bool refreshMsg)
    : m_interval(interval),m_message(message), m_refresh(refreshMsg),m_dialog(new Ui::Dialog)
{
    m_dialog->setupUi(this);

    if(refreshMsg)
    {
        m_timer = new QTimer();

        m_timer->setInterval(second);
        m_timer->start();
        connect(m_timer,SIGNAL(timeout()),this,SLOT(timeOut()));


    }
    m_dialog->textEdit->setText(m_message.arg(m_interval));
}
void TimerDialog::setInterval(int val)
{
    m_interval=val;
}

void TimerDialog::timeOut()
{

    m_dialog->textEdit->setText(m_message.arg(--m_interval));
    if(m_interval==0)
    {
        accept();
    }
}

/*****************
 * ClientServeur *
 *****************/

ClientServeur::ClientServeur()
    : QObject(), m_server(NULL),m_liaisonToServer(NULL)
{
    m_init = Initialisation::getInstance();
    m_reconnect = new QTimer(this);
    m_timerdialog = new TimerDialog(10,tr("Connection fails, retry in %1s."),true);
    connect(m_reconnect, SIGNAL(timeout()), this, SLOT(startConnection()));
}


ClientServeur::~ClientServeur()
{
    // QObject should delete all for us.
}

bool ClientServeur::configAndConnect()
{
    ConnectionConfigDialog configDialog(
        m_init->getUserName(),m_init->getUserColor(),!m_init->isPlayer(),m_init->getIpAddress(), m_init->getServerPort(), !m_init->isClient());


    QMessageBox errorDialog(QMessageBox::Warning, tr("Error"), tr("Can not establish the connection."));


    PlayersList & g_playersList = PlayersList::instance();

    // If the user abort configDialog, we quit
    if (configDialog.exec() == QDialog::Rejected)
        return false;
    synchronizeInitialisation(configDialog);
    m_localPlayer = new Player(
            QUuid(G_idJoueurLocal),
            configDialog.getName(),
            configDialog.getColor(),
            configDialog.isGM()
        );

    bool cont = true;
    while (cont)
    {


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
                //synchronizeInitialisation(configDialog);
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
            m_port = configDialog.getPort();
            m_address = configDialog.getHost();
            if(startConnection())
            {
                cont = false;
            }
        }
        if(cont)
        {
            m_timerdialog->setInterval(10);
            if(m_timerdialog->exec()==QDialog::Rejected)
            {
                exit(0);
            }
        }



    }



    g_playersList.setLocalPlayer(m_localPlayer);
    return true;
}

bool ClientServeur::startConnection()
{
    ConnectionWaitDialog waitDialog;
    QTcpSocket * socket;

    socket = waitDialog.connectTo(m_address, m_port);
    qDebug()<< "connection retry";
    //QMessageBox errorDialog(QMessageBox::Warning, tr("Error"), tr("Can not establish the connection."));
    // connect successed
    if (socket != NULL)
    {
        //G_client=true;
       m_liaisonToServer = new Liaison(socket);
       PlayersList & g_playersList = PlayersList::instance();
       if( m_reconnect->isActive ())
       {
            m_reconnect->stop();
            g_playersList.cleanList();
            g_playersList.sendOffLocalPlayerInformations();
            g_playersList.sendOffFeatures( g_playersList.getLocalPlayer());
       }

       return true;

    }
    else
    {
        sleep(2);
        return false;
    }
}

void ClientServeur::emettreDonnees(char *donnees, quint32 taille, Liaison *sauf)
{
    // Demande d'emission vers toutes les liaisons
    emit emissionDonnees(donnees, taille, sauf);
}


void ClientServeur::ajouterLiaison(Liaison *liaison)
{
    liaisons.append(liaison);
    connect(this, SIGNAL(emissionDonnees(char *, quint32, Liaison *)),liaison, SLOT(emissionDonnees(char *, quint32, Liaison *)));
    connect(liaison, SIGNAL(disconnected(Liaison *)),this, SLOT(finDeLiaison(Liaison *)));
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
        //On quitte l'application
        //G_mainWindow->quitterApplication(true);
        ecrireLogUtilisateur(tr("Receiving picture: %1"));
        if(link!=m_liaisonToServer)
            qDebug() << "link is NOT the link to the server ";
        else
            qDebug() << "link is the link to the server ";

        //startConnection();

        m_reconnect->start(1000);

    }

    // Si l'ordinateur local est le serveur
    else
    {
        if (link == NULL)
        {
            qWarning() << tr("NULL Liaison pointer (ClientServeur::finDeLiaison).");
            return;
        }

        int i = liaisons.indexOf(link);
        if (i < 0)
        {
            qWarning()<< tr("Un thread inconnu vient de se terminer (finDeLiaison - ClientServeur.cpp)");
            return;
        }
        
        // On supprime la liaison de la liste, apres l'avoir detruite
        liaisons.removeAt(i);
    }
}
