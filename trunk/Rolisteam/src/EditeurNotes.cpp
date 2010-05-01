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


	#include <QtGui>

	#include "EditeurNotes.h"
//	#include "variablesGlobales.h"


	/********************************************************************/
	/* Constructeur                                                     */
	/********************************************************************/
    EditeurNotes::EditeurNotes()
        : SubMdiWindows()
    {

		setObjectName("EditeurNotes");
        QWidget* main = new QWidget;
        QVBoxLayout* layout = new QVBoxLayout;




		setWindowIcon(QIcon(":/icones/vignette notes.png"));
		

		notes = new QTextEdit();
		notes->setAcceptRichText(false);


		QObject::connect(notes, SIGNAL(cursorPositionChanged()), this, SLOT(mettreTailleAJour()));




        barreFontes = new QToolBar;

        layout->addWidget(barreFontes);


		barreFontes->setIconSize(QSize(16, 16));


            QAction	*act1 = barreFontes->addAction(QIcon(":/resources/icones/style normal.png"), tr("Normal"), this, SLOT(styleNormal()));
            QAction	*act2 = barreFontes->addAction(QIcon(":/resources/icones/style gras.png"), tr("Gras"), this, SLOT(styleGras()));
            QAction	*act3 = barreFontes->addAction(QIcon(":/resources/icones/style italique.png"), tr("Italique"), this, SLOT(styleItalique()));
            QAction	*act4 = barreFontes->addAction(QIcon(":/resources/icones/style souligne.png"), tr("Souligné"), this, SLOT(styleSouligne()));
            Q_UNUSED(act1)
            Q_UNUSED(act2)
            Q_UNUSED(act3)
            Q_UNUSED(act4)
		#ifdef MACOS

			QPlastiqueStyle *style = new QPlastiqueStyle();
			barreFontes->widgetForAction(act1)->setStyle(style);
			barreFontes->widgetForAction(act2)->setStyle(style);
			barreFontes->widgetForAction(act3)->setStyle(style);
			barreFontes->widgetForAction(act4)->setStyle(style);
		#endif
		

		barreFontes->addSeparator();
		

		selecteurTaille = new QComboBox();
		selecteurTaille->setEditable(false);
		#ifdef WIN32
			selecteurTaille->setFixedWidth(40);
		#elif defined (MACOS)
			selecteurTaille->setFixedWidth(56);
		#endif
		selecteurTaille->setToolTip(tr("Taille de la police"));

		QStringList listeTailles;
		listeTailles << "8" << "10" << "12" << "14" << "16" << "18" << "20" << "22" << "26" << "30" << "40" << "50" << "70";

		selecteurTaille->addItems(listeTailles);

		QObject::connect(selecteurTaille, SIGNAL(activated(int)), this, SLOT(changementTaille(int)));
		

		barreFontes->addWidget(selecteurTaille);
        layout->addWidget(notes);


         main->setLayout(layout);
         setWidget(main);

	}


	void EditeurNotes::closeEvent(QCloseEvent *event)
	{
        Q_UNUSED(event);
		// On masque l'editeur de notes et on decoche l'action associee
        //m_mainWindow->afficherEditeurNotes(false, true);
		// On decoche la case de l'editeur de notes
        //G_listeUtilisateurs->decocherCaseTchat(G_idJoueurLocal);
		// Arret de la procedure de fermeture		

	}

	/********************************************************************/
	/* Met en normal la zone selectionee (ni gras, ni italique)         */
	/********************************************************************/
	void EditeurNotes::styleNormal()
	{
		// On recupere la zone du curseur
		QTextCursor zone = notes->textCursor();
		// On cree un style normal a appliquer a la zone
		QTextCharFormat styleZone;
		styleZone.setFontItalic(false);
		styleZone.setFontUnderline(false);
		styleZone.setFontWeight(QFont::Normal);
		// On ajoute le style a la zone
		zone.mergeCharFormat(styleZone);
		// Enfin on recopie la zone dans les notes
		notes->setTextCursor(zone);
	}
	
	/********************************************************************/
	/* Met en gras la zone selectionee                                  */
	/********************************************************************/
	void EditeurNotes::styleGras()
	{
		// On recupere la zone du curseur
		QTextCursor zone = notes->textCursor();
		// On cree un style gras a appliquer a la zone
		QTextCharFormat styleZone;
		styleZone.setFontWeight(QFont::Bold);
		// On ajoute le style a la zone
		zone.mergeCharFormat(styleZone);
		// Enfin on recopie la zone dans les notes
		notes->setTextCursor(zone);
	}
	
	/********************************************************************/
	/* Met en italique la zone selectionee                              */
	/********************************************************************/
	void EditeurNotes::styleItalique()
	{
		// On recupere la zone du curseur
		QTextCursor zone = notes->textCursor();
		// On cree un style italique a appliquer a la zone
		QTextCharFormat styleZone;
		styleZone.setFontItalic(true);
		// On ajoute le style a la zone
		zone.mergeCharFormat(styleZone);
		// Enfin on recopie la zone dans les notes
		notes->setTextCursor(zone);
	}
	
	/********************************************************************/
	/* Met en souligne la zone selectionee                              */
	/********************************************************************/
	void EditeurNotes::styleSouligne()
	{
		// On recupere la zone du curseur
		QTextCursor zone = notes->textCursor();
		// On cree un style souligne a appliquer a la zone
		QTextCharFormat styleZone;
		styleZone.setFontUnderline(true);
		// On ajoute le style a la zone
		zone.mergeCharFormat(styleZone);
		// Enfin on recopie la zone dans les notes
		notes->setTextCursor(zone);
	}

	/********************************************************************/
	/* Le curseur vient d'etre deplace : on met le selecteur de taille  */
	/* de la fonte a jour                                               */
	/********************************************************************/
	void EditeurNotes::mettreTailleAJour()
	{
		// On recupere la zone du curseur
		QTextCursor zone = notes->textCursor();
		// On recupere le style de la zone
		QTextCharFormat styleActuel = zone.charFormat();
		// On recupere la taille de la fonte a cet endroit
		QFont fonte = styleActuel.font();
		int tailleFonte = fonte.pointSize();
		// On cherche la taille equivalente dans le selecteur de taille
		int index = selecteurTaille->findText(QString::number(tailleFonte));

		// Si la taille a ete trouvee on met a jour le selecteur
		if (index != -1)
			selecteurTaille->setCurrentIndex(index);
		
		// Dans le cas contraire on insere l'element dans la liste
		else
		{
			int i;
			bool trouve = false;
			int tailleListe = selecteurTaille->count();
			// On parcourt la liste des taille jusqu'a ce que l'on trouve une taille superieure
			for (i=0; i<tailleListe && !trouve; i++)
				if (selecteurTaille->itemText(i).toInt() > tailleFonte)
					trouve = true;
			// On met i a jour en fonction du resultat de la recherche
			i = trouve?i-1:i;
			// On insert la nouvelle taille avant l'element en question
			selecteurTaille->insertItem(i, QString::number(tailleFonte));
			// La nouvelle taille est selectionnee
			selecteurTaille->setCurrentIndex(i);
		}
	}

	/********************************************************************/
	/* Modifie la taille de la fonte pour le texte present sous le      */
	/* curseur                                                          */
	/********************************************************************/
	void EditeurNotes::changementTaille(int index)
	{
		// On recupere la taille selectionnee
		int tailleFonte = selecteurTaille->itemText(index).toInt();
		// On recupere la zone du curseur
		QTextCursor zone = notes->textCursor();
		// On recupere le style de la zone
		QTextCharFormat styleActuel = zone.charFormat();
		// On regarde si la fonte est soulignee (l'information n'est pas conservee lors de l'operation)
		bool souligne = styleActuel.fontUnderline();
		// On recupere la fonte a cet endroit
		QFont fonte = styleActuel.font();
		// On met a jour la taille de la fonte
		fonte.setPointSize(tailleFonte);
		// On remet la fonte dans le style
		styleActuel.setFont(fonte);
		// On remet l'etat souligne ou pas
		styleActuel.setFontUnderline(souligne);
		// On met a jour le style de la zone
		zone.mergeCharFormat(styleActuel);
		// Enfin on recopie la zone dans les notes
		notes->setTextCursor(zone);
		// On remet le curseur sur la zone de texte
		notes->setFocus(Qt::OtherFocusReason);
	}
	
	/********************************************************************/
	/* Ouvre les notes contenues dans le fichier passe en parametre     */
	/********************************************************************/
	void EditeurNotes::ouvrirNotes(QFile &file)
    {
		// On cree un flux de donnees rattache au fichier
		QTextStream fichier(&file);
		// On recupere le fichier HTML sous forme de string
		QString html = fichier.readAll();
		// On ecrit le contenu HTML dans les notes
		notes->setHtml(html);
	}

	/********************************************************************/
	/* Sauvegarde les notes dans le fichier passe en parametre          */
	/********************************************************************/
	void EditeurNotes::sauvegarderNotes(QFile &file)
    {
		// On recupere le document contenant les notes
		QTextDocument *document = notes->document();
		// On convertit le document en HTML
		QString html = document->toHtml(QByteArray("UTF-8"));
		// On cree un flux de donnees rattache au fichier
		QTextStream fichier(&file);
		// On envoie le document HTML dans le flux
		fichier << html;
	}

	/********************************************************************/
	/* Renvoie la taille conseillee pour l'editeur de notes             */
	/********************************************************************/
	QSize EditeurNotes::sizeHint() const
	{
		return QSize(600, 600);
	}

