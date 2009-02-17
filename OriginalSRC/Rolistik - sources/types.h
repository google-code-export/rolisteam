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
/* Definition des types utilises dans l'application.                */
/*                                                                  */
/********************************************************************/	


#ifndef TYPES_H
#define TYPES_H

	#include <QColor>
	#include <QString>


	// Types de couleur selectionnables par l'utilisateur
	enum typeCouleur {qcolor, efface, masque, demasque};

	// Definition complete d'une couleur
	typedef struct
	{
		typeCouleur type;
		QColor color;
	} couleurSelectionee;

	// Categories de messages envoyes et recus via les sockets
	enum categorieAction {connexion, joueur, persoJoueur, persoNonJoueur, personnage, dessin, plan, image, discussion, musique, parametres};
	enum actionConnexion {finProcessusConnexion};
	enum actionJoueur {connexionJoueur, ajouterJoueur, supprimerJoueur, changerNomJoueur, changerCouleurJoueur};
	enum actionPj {ajouterPersoJoueur, supprimerPersoJoueur, afficherMasquerPersoJoueur, changerTaillePersoJoueur, changerNomPersoJoueur, changerCouleurPersoJoueur};
	enum actionPnj {ajouterPersoNonJoueur, supprimerPersoNonJoueur};
	enum actionPersonnages {ajouterListePerso, deplacerPerso, changerEtatPerso, changerOrientationPerso, afficherMasquerOrientationPerso};
	enum actionDessin {traceCrayon, traceLigne, traceRectangleVide, traceRectanglePlein, traceEllipseVide, traceEllipsePleine, traceTexte, traceMain};
	enum actionPlan {nouveauPlanVide, chargerPlan, importerPlanComplet, fermerPlan};
	enum actionImage {chargerImage, fermerImage};
	enum actionDiscussion {messageTchat, messageTirage};
	enum actionMusique {nouveauMorceau, lectureMorceau, pauseMorceau, arretMorceau, nouvellePositionMorceau};
	enum actionParametres {changerListeEtats, autoriserDeplacementPersos};
	
	// Entete des messages envoyes et recus via les sockets
	typedef struct
	{ 
		quint8 categorie;				// Contient une categorie d'action
		quint8 action;					// Contient l'action a effectuer
		quint32 tailleDonnees;			// Indique la taille des donnees suivant l'entete
	} enteteMessage;

	// Definition d'un utilisateur
	typedef struct
	{
		QString idJoueur;				// Identifiant du joueur
		QString nomJoueur;				// Nom du joueur
		QColor couleurJoueur;			// Couleur du joueur
		bool mj;						// True si l'utilisateur est un MJ, false sinon
	} utilisateur;

	// Definition du contenu du fichier d'initialisation
	typedef struct
	{
		bool initialisee;				// True si la structure a ete initialisee (le fichier d'initialisation etait present)
		QString versionApplication;		// Version de l'application ayant cree le fichier d'initialisation
		QString nomUtilisateur;			// Nom de l'utilisateur tel qu'indique dans la fenetre de connexion
		QColor couleurUtilisateur;		// Couleur choisie par l'utilisateur
		bool joueur;					// True si l'utilisateur est joueur, false s'il est MJ
		bool client;					// True si l'utilisateur est client, false s'il est serveur
		QString ipServeur;				// Adresse IP du serveur (renseigne par un utilisateur client)
		QString portServeur;			// Port du serveur (renseigne par un utilisateur client)
		QString portClient;				// Port de connexion pour les clients (renseigne par un utilisateur serveur)
		QString dossierMusiquesMj;		// Dossier de chargement des musiques pour le MJ
		QString dossierMusiquesJoueur;	// Dossier de chargement des musiques pour les joueurs
		QString dossierImages;			// Dossier de chargement des images
		QString dossierPlans;			// Dossier de chargement et de sauvegarde des plans
		QString dossierScenarii;		// Dossier de chargement et de sauvegarde des scenarii
		QString dossierNotes;			// Dossier de chargement des notes
		QString dossierTchats;			// Dossier de sauvegarde des tchats
		QColor couleurPersonnelle[16];	// Couleurs personnelles definies par l'utilisateur
		int niveauVolume;				// Volume du lecteur audio
	} initialisation;

#endif
