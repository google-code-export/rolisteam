/*
	Rolistik - logiciel collaboratif d'aide aux jeux de roles en ligne
	Copyright (C) 2007 - Romain Campioni — Tous droits réservés.

	Ce programme est un logiciel libre ; vous pouvez le redistribuer ou le
	modifier suivant les termes de la “GNU General Public License” telle que
	publiée par la Free Software Foundation : soit la version 2 de cette
	licence, soit (à votre gré) toute version ultérieure.

	Ce programme est distribué dans l’espoir qu’il vous sera utile, mais SANS
	AUCUNE GARANTIE : sans même la garantie implicite de COMMERCIALISABILITÉ
	ni d’ADÉQUATION À UN OBJECTIF PARTICULIER. Consultez la Licence Générale
	Publique GNU pour plus de détails.

	Vous devriez avoir reçu une copie de la Licence Générale Publique GNU avec
	ce programme ; si ce n’est pas le cas, consultez :
	<http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>

	Par ailleurs ce logiciel est gratuit et ne peut en aucun cas être
	commercialisé, conformément à la "FMOD Non-Commercial License".
*/


/********************************************************************/
/*                                                                  */
/* Lecteur permettant au MJ de faire jouer de la musique chez les   */
/* autres utilisateurs. Les joueurs ont seuelement la possibilite   */
/* de faire varier le volume de la musique.                         */
/*                                                                  */
/********************************************************************/


#ifndef LECTEUR_AUDIO_H
#define LECTEUR_AUDIO_H

    #include <QWidget>
    #include <QDockWidget>
	#include <QLineEdit>
	#include <QSlider>
	#include <QAction>
	#include <QLCDNumber>
    #include <QListWidget>
    #include <QList>
    #include <QString>
    #include <QVBoxLayout>
    
    #include "types.h"
    #include "fmod.h"
    
	
    class LecteurAudio : public QDockWidget
    {
	Q_OBJECT

    public :
        LecteurAudio(QWidget *parent = 0);
		void arriveeEnFinDeTitre();
		void passageSurUnTag(QString tag);
		void emettreEtat(QString idJoueur);
		void autoriserOuIntedireCommandes();
		void joueurNouveauFichier(QString nomFichier);
		void joueurLectureMorceau();
		void joueurPauseMorceau();
		void joueurArretMorceau();
		void joueurChangerPosition(int position);
		int volume();

	signals :
		void finDeTitreSignal();

    private :
		enum etatLecteur {pause, arret, lecture};
		
		void nouveauTitre(QString titre, QString fichier);
		void arreter();
		void ajouterTags();
		void emettreCommande(actionMusique action, QString nomFichier = "", quint32 position = 0, int numeroLiaison = -1);
		
		FSOUND_STREAM *fluxAudio;		// Contient la reference au flux audio actuellement ouvert
		QWidget *widgetPrincipal;		// Widget contenant tout le lecteur audio (affichage + commande)
		QWidget *widgetAffichage;		// Contient l'afficheur de titre et le reglage du volume (pour joueurs et MJ)
		QWidget *widgetCommande;		// Contient le panneau de commande du lecteur (MJ seulement)
		QVBoxLayout *layoutPrincipal;	// Layout du widget principal
		QLineEdit *afficheurTitre;		// Affiche le titre en cours de lecture
		QSlider *niveauVolume;			// Permet d'ajuster le niveau du volume
		QSlider *positionTemps;			// Permet de voir et de modifier la position de la lecture du titre
		QLCDNumber *afficheurTemps;		// Affiche la position courante du curseur de temps
		QListWidget *listeTitres;		// Contient la liste des titres qui vont etre lus
		QList<QString> listeChemins;	// Liste des chemins des fichiers dont les titres sont affiches dans listeTitres
		QAction *actionLecture;			// Lecture du titre
		QAction *actionPause;			// Mise en pause de la lecture
		QAction *actionStop;			// Arret de la lecture
		QAction *actionBoucle;			// Lecture en boucle du titre
		QAction *actionUnique;			// Lecture unique du titre, sans passer au titre suivant
		QAction *actionAjouter;			// Ajouter un titre a la liste
		QAction *actionSupprimer;		// Supprimer le titre selectionne de la liste
		int etatActuel;					// Etat courant du lecteur
		int titreCourant;				// Numero du titre actuellement lu dans la liste des titres
		int canalAudio;					// Numero de canal utilise pour diffuser le flux audio
		bool enBoucle;					// True si le bouton "Boucle" est enfonce (la lecture du titre a lieu en boucle)
		bool lectureUnique;				// True si le bouton "Lecture unique" est enfonce (apres la lecture en cours, le lecteur s'arrete sans passer au titre suivant)
		bool repriseDeLecture;			// True si la lecture est en cours de lancement (pour eviter qu'un passage sur un tag ne deplace la reprise de lecture)
		int joueurPositionTemps;		// Joueur seulement - indique la position a partir de laquelle la lecture doit commencer

	private slots :
		void appuiPause(bool etatBouton);
		void appuiStop(bool etatBouton);
		void appuiLecture(bool etatBouton);
		void appuiBoucle(bool etatBouton);
		void appuiUnique(bool etatBouton);
		void ajouterTitre();
		void supprimerTitre();
		void changementVolume(int valeur);
		void changementTempsLecture();
		void changementTempsAffichage(int valeur);
		void changementTitre(QListWidgetItem *morceau);
		void finDeTitreSlot();
		void joueurChangerDossier();

	};

#endif
