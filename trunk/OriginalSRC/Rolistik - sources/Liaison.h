/*
	Rolistik - logiciel collaboratif d'aide aux jeux de roles en ligne
	Copyright (C) 2007 - Romain Campioni � Tous droits r�serv�s.

	Ce programme est un logiciel libre ; vous pouvez le redistribuer ou le
	modifier suivant les termes de la �GNU General Public License� telle que
	publi�e par la Free Software Foundation : soit la version 2 de cette
	licence, soit (� votre gr�) toute version ult�rieure.

	Ce programme est distribu� dans l�espoir qu�il vous sera utile, mais SANS
	AUCUNE GARANTIE : sans m�me la garantie implicite de COMMERCIALISABILIT�
	ni d�AD�QUATION � UN OBJECTIF PARTICULIER. Consultez la Licence G�n�rale
	Publique GNU pour plus de d�tails.

	Vous devriez avoir re�u une copie de la Licence G�n�rale Publique GNU avec
	ce programme ; si ce n�est pas le cas, consultez :
	<http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>

	Par ailleurs ce logiciel est gratuit et ne peut en aucun cas �tre
	commercialis�, conform�ment � la "FMOD Non-Commercial License".
*/


/********************************************************************/
/*                                                                  */
/* Chaque Liaison est un thread qui gere un socket, donc la         */
/* reception et l'emission des messages vers les clients ou le      */
/* serveur. En regle generale a la reception d'un message le thread */
/* regarde si l'ordinateur local est le serveur, si c'est le cas il */
/* fait suivre le message aux autres clients.                       */
/*                                                                  */
/********************************************************************/	


#ifndef LIAISON_H
#define LIAISON_H

	#include <QtNetwork>
	#include <QThread>
	#include <QTcpSocket>
	
	#include "Carte.h"
	#include "types.h"
	

    class Liaison : public QThread
    {
	Q_OBJECT

    public :
		Liaison(QTcpSocket *socket, QObject * parent = 0);

	public slots :
		void emissionDonnees(char *donnees, quint32 taille, Liaison *sauf = 0);
		
    signals :

	protected :
		void run();

    private :
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
		void faireSuivreMessage(bool tous);
		void emettreUtilisateur(utilisateur *util, bool multi = false);
		int extrairePersonnage(Carte *carte, char *tampon);

		QTcpSocket *socketTcp;		// Socket gere par le thread
		enteteMessage entete;		// Contient l'entete du message en cours de reception
		bool receptionEnCours;		// Indique si un message est actuellement en cours de reception
		char *tampon;				// Tampon contenant le message en court de reconstitution
		quint32 restant;			// Taille des donnees restant a receptionner
		
	private slots :
		void reception();
		void erreurDeConnexion(QAbstractSocket::SocketError);

	};

#endif
