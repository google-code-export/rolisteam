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
/* Permet de selectionner une couleur dans une palette predefinie   */
/* qui peut etre en partie modifiee.                                */
/*                                                                  */
/********************************************************************/


#ifndef SELECTEUR_COULEUR_H
#define SELECTEUR_COULEUR_H

	#include <QWidget>
	#include <QFrame>
	#include <QColor>
	#include <QLabel>

	#include "types.h"


	class SelecteurCouleur : public QWidget
	{
	public:
		SelecteurCouleur(QWidget *parent = 0);
		void changeCouleurActuelle(QColor couleur);
		void majCouleursPersonnelles();
		void autoriserOuInterdireCouleurs();
		QColor donnerCouleurPersonnelle(int numero);

	private:
		void clicUtilisateur(QPoint positionSouris, bool move = false);

		QLabel *couleurActuelle;
		QLabel *couleurEfface;
		QLabel *couleurMasque;
		QLabel *couleurDemasque;
		QWidget *couleurPredefinie[48];
		QWidget *couleurPersonnelle[16];
		QWidget *separateur1;
		QWidget *separateur2;
		QPixmap *efface_pix;
		QPixmap *masque_pix;
		QPixmap *demasque_pix;
		bool boutonEnfonce;

	protected:
		void mousePressEvent(QMouseEvent *event);		
		void mouseMoveEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		#ifdef MACOS
			void mouseDoubleClickEvent (QMouseEvent *event);
		#endif
	};

#endif
