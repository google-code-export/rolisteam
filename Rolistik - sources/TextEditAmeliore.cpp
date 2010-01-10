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


	#include <QtGui>

	#include "TextEditAmeliore.h"


	/********************************************************************/
	/* Constructeur                                                     */
	/********************************************************************/	
    TextEditAmeliore::TextEditAmeliore(QWidget *parent)
        : QTextEdit(parent)
    {
	}
	
	/********************************************************************/	
	/* Verifie si l'utilisateur a appuye sur la touche Entree. Si oui   */
	/* le signal entreePressee est emis                                 */
	/********************************************************************/	
	void TextEditAmeliore::keyPressEvent(QKeyEvent *e)
	{
		// On recupere la touche qui vient d'etre enfoncee
		int touche = e->key();
		
		// S'il s'agit de la touche Entree ou Return, on emet le signal entreePressee
		if (touche == Qt::Key_Return || touche == Qt::Key_Enter)
			emit entreePressee();

		// S'il s'agit de la touche fleche haut, on emet le signal hautPressee
		else if (touche == Qt::Key_Up)
			emit hautPressee();

		// S'il s'agit de la touche fleche bas, on emet le signal basPressee
		else if (touche == Qt::Key_Down)
			emit basPressee();

		// Sinon on passe l'evenement au QTextEdit
		else
			QTextEdit::keyPressEvent(e);
	}
