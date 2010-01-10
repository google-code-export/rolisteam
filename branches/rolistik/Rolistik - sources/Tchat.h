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
/* Fenetre permettant aux utilisateurs de communiquer entre eux par */
/* ecrit. Il existe un tchat par utilisateur distant, et un tchat   */
/* commun permettant de discuter avec tous les utilisateurs.        */
/*                                                                  */
/********************************************************************/	


#ifndef TCHAT_H
#define TCHAT_H

    #include <QSplitter>
    #include <QFile>

	#include "TextEditAmeliore.h"


    class Tchat : public QSplitter
    {
	Q_OBJECT

    public :
	    Tchat(QString id, QAction *action, QWidget *parent = 0);
	    ~Tchat();
		QString identifiant();
		void afficherMessage(QString utilisateur, QColor couleur, QString message);
	    void afficherTirage(QString utilisateur, QColor couleur, QString message);
		void majAction();
		void sauvegarderTchat(QFile &file);

	protected :
		void closeEvent(QCloseEvent *event);
		void showEvent(QShowEvent *event);

    private :
		int calculerJetDes(QString message, QString *tirage, bool *ok);
		// Ultyme
		int calculerJetDesSR4(QString message, QString *tirage, QString *glitch, bool *ok);
		// FIN Ultyme

		QTextEdit *zoneAffichage;			// Zone de texte ou s'affichent les messages
		TextEditAmeliore *zoneEdition;		// Zone de texte ou l'utilisateur peut ecrire
		QString idJoueur;					// Contient l'ID du jouer a qui doit etre envoye les messages (vide = tous les utilisateurs)
		QList<QString> historiqueMessages;	// Contient l'historique des anciens messages
		int numHistorique;					// Numero de la ligne de l'historique actuellement affichee
		QAction *actionAssociee;			// Action permettant d'afficher/masquer le tchat

    private slots :
		void emettreTexte();
		void monterHistorique();
		void descendreHistorique();
		void changerEtatCase(bool coche);
	};

#endif
