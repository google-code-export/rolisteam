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


/********************************************************************/
/*                                                                  */
/* Zone d'affichage d'une carte : sur ce widget s'affiche la carte  */
/* sur laquelle sont dessinees les annotations. Une couche alpha    */
/* permet de cacher certaines parties de l'image. Une copie         */
/* originale de la carte permet d'effacer les annotations. Les      */
/* personnages sont representes par des widgets DessinPerso.        */
/*                                                                  */
/********************************************************************/


#ifndef CARTE_H
#define CARTE_H

#include <QWidget>
#include <QImage>
#include <QRect>
#include <QPaintEvent>
#include <QMouseEvent>
#include <QPoint>
#include <QPainter>
#include <QTimer>
#include <QFile>

#include "DessinPerso.h"
#include "MainWindow.h"
#include "types.h"
#include "submdiwindows.h"

class MainWindow;
class Carte : public SubMdiWindows
{
    Q_OBJECT

public :
    Carte(QString identCarte, QImage *image, int taillePerso, bool masquer, MainWindow *parent);
    Carte(QString identCarte, QImage *original, QImage *avecAnnotations, QImage *coucheAlpha, int taillePerso, MainWindow *parent);
    void afficheOuMasquePnj(DessinPerso *pnjSeul = 0);
    void changerTaillePjCarte(int nouvelleTaille);
    void affichageDuPj(QString idPerso, bool afficher);
    void emettreCarte(QString titre);
    void emettreCarte(QString titre, int numeroLiaison);
    void emettreTousLesPersonnages();
    void emettreTousLesPersonnages(int numeroLiaison);
    void dessinerTraceCrayon(QList<QPoint> *listePoints, QRect zoneARafraichir, quint8 diametre, couleurSelectionee couleur, bool joueurLocal);
    void dessinerTraceTexte(QString texte, QPoint positionSouris, QRect zoneARafraichir, couleurSelectionee couleur);
    void dessinerTraceGeneral(actionDessin action, QPoint depart, QPoint arrivee, QRect zoneARafraichir, quint8 diametre, couleurSelectionee couleur);
    void adapterCoucheAlpha(quint8 intensiteAlpha);
    void lancerDeplacementPersonnage(QString idPerso, QList<QPoint> listePoints);
    void sauvegarderCarte(QFile &file, QString titre = "");
    int tailleDesPj();
    bool pjAffiche(QString idPerso);
    QString identifiantCarte();
    DessinPerso* trouverPersonnage(QString idPerso);

signals :
    void incrementeNumeroPnj();
    void changeCouleurActuelle(QColor couleur);
    void mettreAJourPnj(int diametre, QString nom);
    void afficherNomsPj(bool afficher);
    void afficherNomsPnj(bool afficher);
    void afficherNumerosPnj(bool afficher);
    void changerTaillePj(int nouvelleTaille);
    void commencerDeplacementCarteFenetre(QPoint position);
    void deplacerCarteFenetre(QPoint position);

public slots :

    /**
      * @brief The drawing cursor becomes the current one.
      */
    void drawingCursor();

    void pointeurTexte();
    void pointeurMain();
    void pointeurAjoutPnj();
    void pointeurSupprPnj();
    void pointeurDeplacePnj();
    void pointeurEtatPnj();
    void ajouterPj(QString idPerso, QString nomPerso, QColor couleur);
    void renommerPj(QString idPerso, QString nouveauNom);
    void effacerPerso(QString idPerso);
    void changerCouleurPj(QString idPerso, QColor couleur);
    void deplacerLesPersonnages();

protected :
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);

private :
    bool ajouterAlpha(QImage *source, QImage *alpha, QImage *destination, const QRect &rect = QRect());
    bool convertirARGB32(QImage *original, QImage *copie);
    QRect zoneARafraichir();
    void initCursors();
    void dessiner(QPainter &painter);
    void emettreTrace();
    void emettreTrajetPersonnage();
    void actionPnjBoutonEnfonce(QPoint positionSouris);
    void actionPnjBoutonRelache(QPoint positionSouris);
    void actionPnjMouvementSouris(QPoint positionSouris);
    void emettreCarteGeneral(QString titre, int numeroLiaison = 0, bool versLiaisonUniquement = false);
    void emettreTousLesPersonnagesGeneral(int numeroLiaison = 0, bool versLiaisonUniquement = false);
    DessinPerso* dansDessinPerso(QPoint positionSouris);

    typedef struct
    {
        QString idPersonnage;
        QList<QPoint> trajet;
    } PersoEnMouvement;

    int taillePj;						// Taille courante des PJ de la carte
    QImage *fond;						// image de fond affichee dans la fenetre
    QImage *fondOriginal;				// image qui servira a effacer les annotations
    QImage *alpha;						// image contenant la couche alpha
    QImage *fondAlpha;					// image temporaire contenant le melange du fond et de la couche alpha
    QImage *effaceAlpha;				// image contenant la couche alpha permettant d'effacer le fond a l'aide du fond original
    bool boutonGaucheEnfonce;			// bouton gauche de la souris enfonce ou pas?
    bool boutonDroitEnfonce;			// bouton droit de la souris enfonce ou pas?
    QPoint pointOrigine;				// point vise au moment du clic gauche
    QPoint pointSouris;					// point actuellement vise par la souris
    QPoint diffSourisDessinPerso;		// difference entre le coin sup gauche du PNJ selectionne (pnjSelectionne) et la position de la souris au moment du clic
    QRect zoneOrigine;					// zone a rafraichir au 1er clic de la souris, puis zone precedemment rafraichie
    QRect zoneNouvelle;					// zone a rafraichir au prochain affichage
    QRect zoneGlobaleCrayon;			// unite de toutes les zone a raffraichir lors du trace du crayon (emise aux autres utilisateurs)
    QCursor pointeur;					// pointeur actuel de la souris
    DessinPerso *pnjSelectionne;		// pointe sur le PNJ actuellement selectionne (0 si aucun PNJ selectionne)
    DessinPerso *dernierPnjSelectionne;	// pointe sur le dernier PNJ selectionne (0 si aucun PNJ n'a deja ete selection)
    QString idCarte;					// identifiant de la carte
    QList<QPoint> listePointsCrayon;	// liste des points composant le trace du crayon, qui sera emise aux autres utilisateurs
    QList<QPoint> listeDeplacement;		// liste des points composant le deplacement du perso qui vient d'etre deplace par l'utilisateur
    QList<PersoEnMouvement> mouvements;	// liste des personnages a deplacer, ainsi que leur trajectoire


    /**
      * MainWindow the parent of all maps
      */
    MainWindow* m_mainWindow;

};

#endif
