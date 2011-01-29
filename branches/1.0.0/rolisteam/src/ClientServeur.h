/*************************************************************************
 *     Copyright (C) 2007 by Romain Campioni                                *
 *     Copyright (C) 2009 by Renaud Guezennec                              *
 *   Copyrigth (C) 2010 by Joseph Boudou                                 *
 *                                                                       *
 *   http://www.rolisteam.org/                                           *
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


#ifndef CLIENT_SERVEUR_H
#define CLIENT_SERVEUR_H

#include <QTcpServer>
#include <QList>
#include <QColor>

class PlayerTransfer
{
public:
    PlayerTransfer(QString id,QString name,QColor color,bool gm);


    QString name();
    QColor color();
    bool isGM();
    QString id();

private:
    QString m_id;
    QString m_name;
    QColor m_color;
    bool m_GM;
//G_idJoueurLocal, configDialog.getName(), configDialog.getColor(), true, configDialog.isGM()
};


class Liaison;
/**
 * @brief hold the list of socket (Liaison).
 * On startup displays the configDialog.
 */
class ClientServeur
    : public QObject
{
    Q_OBJECT

public :
    ClientServeur();
    ~ClientServeur();

    /**
     * @brief Display the configDialog and make the connection.
     * @return true if connection has been established, false if the user has clicked on the Quit button.
     */
    bool configAndConnect();

    void emettreDonnees(char *donnees, quint32 taille, Liaison *sauf);
    void emettreDonnees(char *donnees, quint32 taille, int numeroLiaison);

    void ajouterLiaison(Liaison *liaison);

    PlayerTransfer* currentUser();

signals :
    void emissionDonnees(char *donnees, quint32 taille, Liaison *sauf);

private :
    QTcpServer * m_server;
    QList<Liaison *> liaisons;
    PlayerTransfer* m_user;


private slots :
    void nouveauClientConnecte();
    void finDeLiaison();
};

#endif