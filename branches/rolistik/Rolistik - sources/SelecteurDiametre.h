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
/* Dessine un disque dont le diametre varie grace a un QSlider.     */
/*                                                                  */
/********************************************************************/


#ifndef SELECTEUR_DIAMETRE_H
#define SELECTEUR_DIAMETRE_H

	#include <QWidget>
	#include <QSlider>
	
	#include "AfficheurDisque.h"

	
	class SelecteurDiametre : public QWidget
	{
	public :
		SelecteurDiametre(QWidget *parent = 0, bool plein = true, int minimum = 1, int maximum = 50);
		void changerDiametre(int nouvelleValeur);

	private :
		int minimum;
		int maximum;
		AfficheurDisque *disque;
		QSlider *diametre;
	};

#endif
