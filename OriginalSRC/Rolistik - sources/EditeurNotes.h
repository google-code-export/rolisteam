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
/* Editeur de notes permettant aux utilisateurs de prendre des      */
/* notes en partie                                                  */
/*                                                                  */
/********************************************************************/	


#ifndef EDITEUR_NOTES_H
#define EDITEUR_NOTES_H

    #include <QWidget>
    #include <QMainWindow>
    #include <QTextEdit>
    #include <QCloseEvent>
    #include <QToolBar>
	#include <QComboBox>
	#include <QFile>


    class EditeurNotes : public QMainWindow
    {
	Q_OBJECT

    public :
	    EditeurNotes();
	    void sauvegarderNotes(QFile &file);
	    void ouvrirNotes(QFile &file);
		QSize sizeHint() const;

    private :
		QTextEdit *notes;			// Contient toutes les notes de l'utilisateur
		QToolBar *barreFontes;		// Contient les boutons de formatage du texte
		QComboBox *selecteurTaille;	// Permet de choisir la taille de la fonte

    private slots :
		void styleNormal();
		void styleGras();
		void styleItalique();
		void styleSouligne();
		void mettreTailleAJour();
		void changementTaille(int index);

	protected :
		void closeEvent(QCloseEvent *event);

	};

#endif
