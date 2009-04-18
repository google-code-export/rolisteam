/*
	Rolistik - logiciel collaboratif d'aide aux jeux de roles en ligne
	Copyright (C) 2007 - Romain Campioni  Tous droits rservs.

	Ce programme est un logiciel libre ; vous pouvez le redistribuer ou le
	modifier suivant les termes de la GNU General Public License telle que
	publie par la Free Software Foundation : soit la version 2 de cette
	licence, soit ( votre gr) toute version ultrieure.

	Ce programme est distribu dans lespoir quil vous sera utile, mais SANS
	AUCUNE GARANTIE : sans mme la garantie implicite de COMMERCIALISABILIT
	ni dADQUATION  UN OBJECTIF PARTICULIER. Consultez la Licence Gnrale
	Publique GNU pour plus de dtails.

	Vous devriez avoir reu une copie de la Licence Gnrale Publique GNU avec
	ce programme ; si ce nest pas le cas, consultez :
	<http://www.gnu.org/licenses/old-licenses/gpl-2.0.html>

	Par ailleurs ce logiciel est gratuit et ne peut en aucun cas tre
	commercialis, conformment  la "FMOD Non-Commercial License".
*/


	#include <QtGui>

	#include "BarreOutils.h"
	#include "SelecteurCouleur.h"
	#include "SelecteurDiametre.h"
	#include "constantesGlobales.h"
	
	
	/********************************************************************/
	/* Variables globales utilisees par tous les elements de            */
	/* l'application                                                    */
	/********************************************************************/	
	// Definit l'outil courant
	BarreOutils::outilSelectionne G_outilCourant;
	// Contient le texte de la zone de texte
	QString G_texteCourant;
	// Contient le texte de la zone "nom du PNJ"
	QString G_nomPnjCourant;
	// Numero de PNJ courant
	int G_numeroPnjCourant;
	
	
	/********************************************************************/
	/* Constructeur                                                     */
	/********************************************************************/	
	BarreOutils::BarreOutils(QWidget *parent)
		: QDockWidget(parent)
	{
		// Titre du dockWidget
		setWindowTitle(tr("Outils"));
		// Parametrage du dockWidget
        setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
		setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);
		// Creation du widget contenant les boutons
		outils = new QWidget(this);
		// Insertion du widget dans le dockWidget
		setWidget(outils);

		// Creation des actions
		creerActions();
		// Creation des outils
		creerOutils();

		// Initialisation de la variable globale indiquant l'outil courant
		G_outilCourant = main;
		
		// Connexion de l'action RazChrono avec le slot razNumeroPnj
		QObject::connect(actionRazChrono, SIGNAL(triggered(bool)), this, SLOT(razNumeroPnj()));
		// Connexion du changement de la zone de texte avec texteChange
		QObject::connect(ligneDeTexte, SIGNAL(textEdited(const QString &)), this, SLOT(texteChange(const QString &)));
		// Connexion du changement du nom de PNJ avec nomPnjChange
		QObject::connect(nomPnj, SIGNAL(textEdited(const QString &)), this, SLOT(nomPnjChange(const QString &)));
		// Connexion des actions avec les slot adaptes
		QObject::connect(actionCrayon,     SIGNAL(triggered(bool)), this, SLOT(crayonSelectionne()));
		QObject::connect(actionLigne,      SIGNAL(triggered(bool)), this, SLOT(ligneSelectionne()));
		QObject::connect(actionRectVide,   SIGNAL(triggered(bool)), this, SLOT(rectVideSelectionne()));
		QObject::connect(actionRectPlein,  SIGNAL(triggered(bool)), this, SLOT(rectPleinSelectionne()));
		QObject::connect(actionElliVide,   SIGNAL(triggered(bool)), this, SLOT(elliVideSelectionne()));
		QObject::connect(actionElliPlein,  SIGNAL(triggered(bool)), this, SLOT(elliPleinSelectionne()));
		QObject::connect(actionTexte,      SIGNAL(triggered(bool)), this, SLOT(texteSelectionne()));
		QObject::connect(actionMain,       SIGNAL(triggered(bool)), this, SLOT(mainSelectionne()));
		QObject::connect(actionAjoutPnj,   SIGNAL(triggered(bool)), this, SLOT(ajoutPnjSelectionne()));
		QObject::connect(actionSupprPnj,   SIGNAL(triggered(bool)), this, SLOT(supprPnjSelectionne()));
		QObject::connect(actionDeplacePnj, SIGNAL(triggered(bool)), this, SLOT(deplacePersoSelectionne()));
		QObject::connect(actionEtatPnj,    SIGNAL(triggered(bool)), this, SLOT(etatPersoSelectionne()));
		// Connection du changement d'etat (floating / no floating) avec le changement de taille
		QObject::connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(changementTaille(bool)));

		setFloating(false);
	}

	/********************************************************************/	
	/* Autorise ou pas la selection des couleurs de masquage et         */
	/* demasquage en fonction de la nature de l'utilisateur (MJ/joueur) */
	/********************************************************************/	
	void BarreOutils::autoriserOuInterdireCouleurs()
	{
		couleur->autoriserOuInterdireCouleurs();
	}

	/********************************************************************/
	/* Creation des actions                                             */
	/********************************************************************/	
	void BarreOutils::creerActions()
	{
		// Creation du groupe d'action
		QActionGroup *groupOutils = new QActionGroup(this);

		// Creation des actions
                actionCrayon 	= new QAction(QIcon(":/resources/icones/crayon.png"), tr("Crayon"), groupOutils);
                actionLigne 	= new QAction(QIcon(":/resources/icones/ligne.png"), tr("Ligne"), groupOutils);
                actionRectVide 	= new QAction(QIcon(":/resources/icones/rectangle vide.png"), tr("Rectangle vide"), groupOutils);
                actionRectPlein	= new QAction(QIcon(":/resources/icones/rectangle plein.png"), tr("Rectangle plein"), groupOutils);
                actionElliVide 	= new QAction(QIcon(":/resources/icones/ellipse vide.png"), tr("Ellipse vide"), groupOutils);
                actionElliPlein	= new QAction(QIcon(":/resources/icones/ellipse pleine.png"), tr("Ellipse pleine"), groupOutils);
                actionTexte 	= new QAction(QIcon(":/resources/icones/texte.png"), tr("Texte"), groupOutils);
                actionMain		= new QAction(QIcon(":/resources/icones/main.png"), tr("Déplacer"), groupOutils);
                actionAjoutPnj 	= new QAction(QIcon(":/resources/icones/ajouter PNJ.png"), tr("Ajouter un PNJ"), groupOutils);
                actionSupprPnj 	= new QAction(QIcon(":/resources/icones/supprimer PNJ.png"), tr("Supprimer un PNJ"), groupOutils);
                actionDeplacePnj= new QAction(QIcon(":/resources/icones/deplacer PNJ.png"), tr("Déplacer/Orienter un personnage"), groupOutils);
                actionEtatPnj	= new QAction(QIcon(":/resources/icones/etat.png"), tr("Changer l'état d'un personnage"), groupOutils);

		// Action independante : remise a 0 des numeros de PNJ
                actionRazChrono	= new QAction(QIcon(":/resources/icones/chronometre.png"), tr("RAZ numéros de PNJ"), this);

		// Les actions sont checkable
		actionCrayon	->setCheckable(true);
		actionLigne		->setCheckable(true);
		actionRectVide	->setCheckable(true);
		actionRectPlein	->setCheckable(true);
		actionElliVide	->setCheckable(true);
		actionElliPlein	->setCheckable(true);
		actionTexte		->setCheckable(true);
		actionMain		->setCheckable(true);
		actionAjoutPnj	->setCheckable(true);
		actionSupprPnj	->setCheckable(true);
		actionDeplacePnj->setCheckable(true);
		actionEtatPnj	->setCheckable(true);

		// Choix d'une action selectionnee au depart
		actionMain->setChecked(true);
	}	

	/********************************************************************/
	/* Creation des boutons et du widget qui les contient               */
	/********************************************************************/	
	void BarreOutils::creerOutils()
	{
		// Creation des boutons du toolBar
		QToolButton *boutonCrayon     = new QToolButton(outils);
		QToolButton *boutonLigne      = new QToolButton(outils);
		QToolButton *boutonRectVide   = new QToolButton(outils);
		QToolButton *boutonRectPlein  = new QToolButton(outils);
		QToolButton *boutonElliVide   = new QToolButton(outils);
		QToolButton *boutonElliPlein  = new QToolButton(outils);
		QToolButton *boutonTexte      = new QToolButton(outils);
		QToolButton *boutonMain       = new QToolButton(outils);
		QToolButton *boutonAjoutPnj   = new QToolButton(outils);
		QToolButton *boutonSupprPnj   = new QToolButton(outils);
		QToolButton *boutonDeplacePnj = new QToolButton(outils);
		QToolButton *boutonEtatPnj    = new QToolButton(outils);
		QToolButton *boutonRazChrono  = new QToolButton(outils);

		// Association des boutons avec les actions
		boutonCrayon     ->setDefaultAction(actionCrayon);
		boutonLigne      ->setDefaultAction(actionLigne);
		boutonRectVide   ->setDefaultAction(actionRectVide);
		boutonRectPlein  ->setDefaultAction(actionRectPlein);
		boutonElliVide   ->setDefaultAction(actionElliVide);
		boutonElliPlein  ->setDefaultAction(actionElliPlein);
		boutonTexte      ->setDefaultAction(actionTexte);
		boutonMain       ->setDefaultAction(actionMain);
		boutonAjoutPnj   ->setDefaultAction(actionAjoutPnj);
		boutonSupprPnj   ->setDefaultAction(actionSupprPnj);
		boutonDeplacePnj ->setDefaultAction(actionDeplacePnj);
		boutonEtatPnj    ->setDefaultAction(actionEtatPnj);
		boutonRazChrono  ->setDefaultAction(actionRazChrono);

		// Boutons en mode AutoRaise, plus lisible
		boutonCrayon     ->setAutoRaise(true);
		boutonLigne      ->setAutoRaise(true);
		boutonRectVide   ->setAutoRaise(true);
		boutonRectPlein  ->setAutoRaise(true);
		boutonElliVide   ->setAutoRaise(true);
		boutonElliPlein  ->setAutoRaise(true);
		boutonTexte      ->setAutoRaise(true);
		boutonMain       ->setAutoRaise(true);
		boutonAjoutPnj   ->setAutoRaise(true);
		boutonSupprPnj   ->setAutoRaise(true);
		boutonDeplacePnj ->setAutoRaise(true);
		boutonEtatPnj    ->setAutoRaise(true);
		boutonRazChrono  ->setAutoRaise(true);

		#ifdef MACOS
			// Changement du style des boutons (plus lisible)
			QPlastiqueStyle *styleBouton = new QPlastiqueStyle();
			boutonCrayon     ->setStyle(styleBouton);
			boutonLigne      ->setStyle(styleBouton);
			boutonRectVide   ->setStyle(styleBouton);
			boutonRectPlein  ->setStyle(styleBouton);
			boutonElliVide   ->setStyle(styleBouton);
			boutonElliPlein  ->setStyle(styleBouton);
			boutonTexte      ->setStyle(styleBouton);
			boutonMain       ->setStyle(styleBouton);
			boutonAjoutPnj   ->setStyle(styleBouton);
			boutonSupprPnj   ->setStyle(styleBouton);
			boutonDeplacePnj ->setStyle(styleBouton);
			boutonEtatPnj    ->setStyle(styleBouton);
			boutonRazChrono  ->setStyle(styleBouton);
		#endif
		
		// Changement de la taille des icones
		QSize tailleIcones(TAILLE_ICONES,TAILLE_ICONES);
		boutonCrayon     ->setIconSize(tailleIcones);
		boutonLigne      ->setIconSize(tailleIcones);
		boutonRectVide   ->setIconSize(tailleIcones);
		boutonRectPlein  ->setIconSize(tailleIcones);
		boutonElliVide   ->setIconSize(tailleIcones);
		boutonElliPlein  ->setIconSize(tailleIcones);
		boutonTexte      ->setIconSize(tailleIcones);
		boutonMain       ->setIconSize(tailleIcones);
		boutonAjoutPnj   ->setIconSize(tailleIcones);
		boutonSupprPnj   ->setIconSize(tailleIcones);
		boutonDeplacePnj ->setIconSize(tailleIcones);
		boutonEtatPnj    ->setIconSize(tailleIcones);
		boutonRazChrono  ->setIconSize(tailleIcones);
					
		// Creation du layout vertical qui constitue la barre d'outils
		QVBoxLayout *outilsLayout = new QVBoxLayout(outils);
		outilsLayout->setSpacing(0);
		outilsLayout->setMargin(2);

		// Creation du layout qui contient les outils de dessin
		QGridLayout *layoutDessin = new QGridLayout();
		layoutDessin->setSpacing(0);
		layoutDessin->setMargin(0);
		layoutDessin->addWidget(boutonCrayon, 0, 0);
		layoutDessin->addWidget(boutonLigne, 0, 1);
		layoutDessin->addWidget(boutonRectVide, 1, 0);
		layoutDessin->addWidget(boutonRectPlein, 1, 1);
		layoutDessin->addWidget(boutonElliVide, 2, 0);
		layoutDessin->addWidget(boutonElliPlein, 2, 1);
		layoutDessin->addWidget(boutonTexte, 3, 0);
		layoutDessin->addWidget(boutonMain, 3, 1);

		// Creation des zones de texte et de nom de PNJ
		ligneDeTexte = new QLineEdit(outils);
		ligneDeTexte->setToolTip(tr("Texte"));

		nomPnj = new QLineEdit(outils);
		nomPnj->setToolTip(tr("Nom du PNJ"));

		#ifdef MACOS
			ligneDeTexte->setFixedHeight(22);
			nomPnj->setFixedHeight(22);
		#endif
		
		// Creation de l'afficheur du numero de PNJ
		afficheNumeroPnj = new QLCDNumber(2, outils);
		afficheNumeroPnj->setSegmentStyle(QLCDNumber::Flat);
		afficheNumeroPnj->setMaximumSize(TAILLE_ICONES + 7, TAILLE_ICONES);
		afficheNumeroPnj->display(1);
                afficheNumeroPnj->setToolTip(tr("Numéro de PNJ"));
		// Initialisation de la variable globale indiquant le numero de PNJ courant
		G_numeroPnjCourant = 1;
		
		// Creation du selecteur de couleur
		couleur = new SelecteurCouleur(outils);

		// Creation du layout contient les outils de deplcement des PNJ
		QHBoxLayout *layoutMouvementPnj = new QHBoxLayout();
		layoutMouvementPnj->setSpacing(0);
		layoutMouvementPnj->setMargin(0);
		layoutMouvementPnj->addWidget(boutonDeplacePnj);
		layoutMouvementPnj->addWidget(boutonEtatPnj);

		// Creation du layout contient les outils d'ajout et de suppression des PNJ
		QGridLayout *layoutAjoutPnj = new QGridLayout();
		layoutAjoutPnj->setSpacing(0);
		layoutAjoutPnj->setMargin(0);
		layoutAjoutPnj->addWidget(boutonAjoutPnj, 0, 0);
		layoutAjoutPnj->addWidget(boutonSupprPnj, 0, 1);
		layoutAjoutPnj->addWidget(boutonRazChrono, 1, 0);
		layoutAjoutPnj->addWidget(afficheNumeroPnj, 1, 1, Qt::AlignHCenter);
		
		// Creation du selecteur de diametre du trait
		diametreTrait = new SelecteurDiametre(outils, true, 1, 45);
		diametreTrait->setToolTip(tr("Grosseur du trait"));

		// Creation du selecteur de diametre des PNJ
		diametrePnj = new SelecteurDiametre(outils, false, 12, 41);
		diametrePnj->setToolTip(tr("Taille du PNJ"));

		//Creation du separateur se trouvant entre le selecteur de couleur et les outils de dessin
		QFrame *separateur1 = new QFrame(outils);
		separateur1->setFrameStyle(QFrame::HLine | QFrame::Sunken);
		separateur1->setMinimumHeight(15);
		separateur1->setLineWidth(1);
		separateur1->setMidLineWidth(0);
		
		// Creation du separateur se trouvant entre les outils de dessin et le selecteur de diametre du trait
		QWidget *separateur2 = new QWidget(outils);
		separateur2->setFixedHeight(3);
		
		//Creation du separateur se trouvant entre les outils de dessin et ceux de deplacement des PNJ
		QFrame *separateur3 = new QFrame(outils);
		separateur3->setFrameStyle(QFrame::HLine | QFrame::Sunken);
		separateur3->setMinimumHeight(10);
		separateur3->setLineWidth(1);
		separateur3->setMidLineWidth(0);

		//Creation du separateur se trouvant entre les outils de deplacement et ceux d'ajout des PNJ
		QFrame *separateur4 = new QFrame(outils);
		separateur4->setFrameStyle(QFrame::HLine | QFrame::Sunken);
		separateur4->setMinimumHeight(10);
		separateur4->setLineWidth(1);
		separateur4->setMidLineWidth(0);

		// Creation du separateur se trouvant au dessus du selecteur de diametre des PNJ
		QWidget *separateur5 = new QWidget(outils);
		separateur5->setFixedHeight(3);

		#ifdef MACOS
			// Creation des separateurs se trouvant au dessus des zones de saisie de texte
			QWidget *separateur10 = new QWidget(outils);
			QWidget *separateur11 = new QWidget(outils);
			separateur10->setFixedHeight(3);
			separateur11->setFixedHeight(3);
		#endif
		
		// Ajout des differents layouts et widgets dans outilsLayout
		outilsLayout->addWidget(couleur);
		outilsLayout->addWidget(separateur1);
		outilsLayout->addLayout(layoutDessin);
		#ifdef MACOS
			outilsLayout->addWidget(separateur10);
		#endif
		outilsLayout->addWidget(ligneDeTexte);
		outilsLayout->addWidget(separateur2);
		outilsLayout->addWidget(diametreTrait);
		outilsLayout->addWidget(separateur3);
		outilsLayout->addLayout(layoutMouvementPnj);
		outilsLayout->addWidget(separateur4);
		outilsLayout->addLayout(layoutAjoutPnj);
		#ifdef MACOS
			outilsLayout->addWidget(separateur11);
		#endif
		outilsLayout->addWidget(nomPnj);
		outilsLayout->addWidget(separateur5);
		outilsLayout->addWidget(diametrePnj);

		// Alignement du widget outils sur le haut du dockWidget
		layout()->setAlignment(outils, Qt::AlignTop | Qt::AlignHCenter);
		// Contraintes de taille sur la barre d'outils
		outils->setFixedWidth((TAILLE_ICONES+8)*layoutDessin->columnCount());
		setMaximumWidth((TAILLE_ICONES+8)*layoutDessin->columnCount()+10);
	}

	/********************************************************************/
	/* Incrementation du numero de PNJ                                   */
	/********************************************************************/	
	void BarreOutils::incrementeNumeroPnj()
	{
		// Recuperation de la valeur actuelle
		int numeroActuel = (int) afficheNumeroPnj->value();
		
		// Incrementation
		numeroActuel++;
		// MAJ de la valeur; si la nouvelle valeur ne rentre pas, on la met a 1
		if (afficheNumeroPnj->checkOverflow(numeroActuel))
			afficheNumeroPnj->display(1);
		else
			afficheNumeroPnj->display(numeroActuel);
			
		// Mise a jour de la variable globale indiquant le numero de PNJ
		G_numeroPnjCourant = (int) afficheNumeroPnj->value();
	}

	/********************************************************************/
	/* Remise a 1 du numero de PNJ                                      */
	/********************************************************************/	
	void BarreOutils::razNumeroPnj()
	{
		afficheNumeroPnj->display(1);
		// Mise a jour de la variable globale indiquant le numero de PNJ
		G_numeroPnjCourant = 1;
	}

	/********************************************************************/
	/* Changement de taille en fonction de l'etat du dockwidget         */
	/********************************************************************/	
	void BarreOutils::changementTaille(bool floating)
	{
                if (floating)
                {
			#ifdef WIN32
				setFixedHeight(578);
			#elif defined (MACOS)
				setFixedHeight(570);
                        #endif
                }
		else
			setMaximumHeight(0xFFFFFF);
	}

	/********************************************************************/
	/* Mise a jour de la variable globale contenant le texte            */
	/********************************************************************/	
	void BarreOutils::texteChange(const QString &texte)
	{
		// M.a.j automatique de la variable globale contenant le texte
		G_texteCourant = texte;
		// Selection automatique de l'action Texte
		actionTexte->trigger();
	}

	/********************************************************************/
	/* Mise a jour de la variable globale contenant le nom du PNJ       */
	/********************************************************************/	
	void BarreOutils::nomPnjChange(const QString &texte)
	{
		// M.a.j de la variable globale contenant le nom du PNJ
		G_nomPnjCourant = texte;
		// Selection automatique de l'action ajout PNJ
		actionAjoutPnj->trigger();
	}

	/********************************************************************/
	/* Fait suivre une demande de changement de couleur au selecteur    */
	/* de couleur                                                       */
	/********************************************************************/
	void BarreOutils::changeCouleurActuelle(QColor coul)
	{
		couleur->changeCouleurActuelle(coul);
	}

	/********************************************************************/
	/* M.a.j le nom et le diametre du PNJ                               */
	/********************************************************************/
	void BarreOutils::mettreAJourPnj(int diametre, QString nom)
	{
		// M.a.j du diametre du PNJ (ce qui met a jour la variable globale)
		diametrePnj->changerDiametre(diametre);
		// M.a.j de la zone de texte contenant le nom du PNJ
		nomPnj->setText(nom);
		// M.a.j de la variable globale contenant le nom du PNJ
		G_nomPnjCourant = nom;
	}

	/********************************************************************/	
	/* Demande une m.a.j des couleurs personnelles au selecteur de      */
	/* couleurs                                                         */
	/********************************************************************/	
	void BarreOutils::majCouleursPersonnelles()
	{
		couleur->majCouleursPersonnelles();
	}

	/********************************************************************/	
	/* Renvoie la couleur personnelle dont le numero est passe en       */
	/* parametre                                                        */
	/********************************************************************/	
	QColor BarreOutils::donnerCouleurPersonnelle(int numero)
	{
		return couleur->donnerCouleurPersonnelle(numero);
	}

	/********************************************************************/
	/* Selectionne l'outil crayon                                       */
	/********************************************************************/	
	void BarreOutils::crayonSelectionne()
	{
		G_outilCourant = crayon;
	}
	
	/********************************************************************/
	/* Selectionne l'outil ligne                                        */
	/********************************************************************/	
	void BarreOutils::ligneSelectionne()
	{
		G_outilCourant = ligne;
	}

	/********************************************************************/
	/* Selectionne l'outil rectangle vide                               */
	/********************************************************************/	
	void BarreOutils::rectVideSelectionne()
	{
		G_outilCourant = rectVide;
	}

	/********************************************************************/
	/* Selectionne l'outil rectangle plein                              */
	/********************************************************************/	
	void BarreOutils::rectPleinSelectionne()
	{
		G_outilCourant = rectPlein;
	}

	/********************************************************************/
	/* Selectionne l'outil ellipse vide                                 */
	/********************************************************************/	
	void BarreOutils::elliVideSelectionne()
	{
		G_outilCourant = elliVide;
	}

	/********************************************************************/
	/* Selectionne l'outil ellipse pleine                               */
	/********************************************************************/	
	void BarreOutils::elliPleinSelectionne()
	{
		G_outilCourant = elliPlein;
	}

	/********************************************************************/
	/* Selectionne l'outil texte                                        */
	/********************************************************************/	
	void BarreOutils::texteSelectionne()
	{
		// Le focus du clavier est oriente vers la zone de texte
		if (!(ligneDeTexte->hasFocus()))
		{
			ligneDeTexte->setFocus(Qt::OtherFocusReason);
			ligneDeTexte->setSelection(0, G_texteCourant.length());
		}
		G_outilCourant = texte;
	}

	/********************************************************************/
	/* Selectionne l'outil main                                         */
	/********************************************************************/	
	void BarreOutils::mainSelectionne()
	{
		G_outilCourant = main;
	}

	/********************************************************************/
	/* Selectionne l'outil ajouter PNJ                                  */
	/********************************************************************/	
	void BarreOutils::ajoutPnjSelectionne()
	{
		// Le focus du clavier est oriente vers la zone de nom de PNJ
		if (!(nomPnj->hasFocus()))
		{
			nomPnj->setFocus(Qt::OtherFocusReason);
			nomPnj->setSelection(0, G_nomPnjCourant.length());
		}
		G_outilCourant = ajoutPnj;
	}

	/********************************************************************/
	/* Selectionne l'outil supprimer PNJ                                */
	/********************************************************************/	
	void BarreOutils::supprPnjSelectionne()
	{
		G_outilCourant = supprPnj;
	}

	/********************************************************************/
	/* Selectionne l'outil deplacer PNJ                                 */
	/********************************************************************/	
	void BarreOutils::deplacePersoSelectionne()
	{
		G_outilCourant = deplacePerso;
	}

	/********************************************************************/
	/* Selectionne l'outil changer etat PNJ                             */
	/********************************************************************/	
	void BarreOutils::etatPersoSelectionne()
	{
		G_outilCourant = etatPerso;
	}
