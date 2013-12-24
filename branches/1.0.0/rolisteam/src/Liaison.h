/*************************************************************************
 *   Copyright (C) 2007 by Romain Campioni                               *
 *   Copyright (C) 2009 by Renaud Guezennec                              *
 *   Copyright (C) 2011 by Joseph Boudou                                 *
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


#ifndef LIAISON_H
#define LIAISON_H

#include <QtNetwork>
#include <QTcpSocket>

#include "networkmessage.h"
#include "mainwindow.h"

class Carte;
#ifndef NULL_PLAYER
    class LecteurAudio;
#endif

/**
 * @brief The Liaison class
 */
class Liaison : public QObject
{
Q_OBJECT

public :
    Liaison(QTcpSocket *socket);
    ~Liaison();


    void setSocket(QTcpSocket* socket, bool makeConnection = true);

    void disconnectAndClose();

    void initialize();
signals:
    void disconnected(Liaison * link);

public slots :
    void emissionDonnees(char *donnees, quint32 taille, Liaison *sauf = 0);
    void setMainWindow(MainWindow* mainWindow);

private :
    void makeSignalConnection();
    void receptionMessageConnexion();
    void receptionMessageJoueur();
    void receptionMessagePersoJoueur();
    void receptionMessagePersoNonJoueur();
    void receptionMessagePersonnage();
    void receptionMessageDessin();
    void receptionMessagePlan();
    void receptionMessageImage();
    void receptionMessageDiscussion();
    void receptionMessageMusique();
    void receptionMessageParametres();

    void postTo(QObject * obj) const;
    void faireSuivreMessage(bool tous);
    int extrairePersonnage(Carte *carte, char *tampon);

    QTcpSocket* m_socketTcp;		 // Socket gere par le thread
    NetworkMessageHeader entete; // Contient l'entete du message en cours de reception
    bool receptionEnCours;		 // Indique si un message est actuellement en cours de reception
    char *tampon;				 // Tampon contenant le message en court de reconstitution
    quint32 restant;			 // Taille des donnees restant a receptionner
#ifndef NULL_PLAYER
    LecteurAudio* m_audioPlayer;
#endif
    MainWindow* m_mainWindow;
    ClientServeur* m_networkManager;

private slots :
    void reception();
    void erreurDeConnexion(QAbstractSocket::SocketError);
    void p_disconnect();
};

#endif
