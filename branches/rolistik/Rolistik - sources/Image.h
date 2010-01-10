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
/* QScrollArea contenant une image, qui ne peut etre fermee que par */
/* le joueur qui l'a ouverte ou par le MJ. Le but est de pouvoir    */
/* afficher des images sans passer par les cartes qui utilisent     */
/* beaucoup de ressources systeme.                                  */
/*                                                                  */
/********************************************************************/	


#ifndef IMAGE_H
#define IMAGE_H

    #include <QWidget>
    #include <QImage>
    #include <QScrollArea>
    #include <QAction>
	#include <QFile>
	#include <QString>
	#include <QLabel>


    class Image : public QScrollArea
    {
	Q_OBJECT
		
    public :
	    Image(QString identImage, QString identJoueur, QImage *image, QAction *action = 0, QWidget *parent = 0);
		~Image();
		void associerAction(QAction *action);
		void emettreImage(QString titre, int numeroLiaison);
		void sauvegarderImage(QFile &file, QString titre);
		bool proprietaireImage();
		QString identifiantImage();

	public slots :
		void pointeurMain();
		void pointeurNormal();

    private :
		QString idImage;
		QString idJoueur;
		QAction *actionAssociee;
		QLabel *labelImage;
		QPoint pointDepart;
		int horizontalDepart;
		int verticalDepart;
		bool deplacementAutorise;

	protected :
		void closeEvent(QCloseEvent *event);
		void mousePressEvent(QMouseEvent *event);
		void mouseReleaseEvent(QMouseEvent *event);
		void mouseMoveEvent(QMouseEvent *event);

	};

#endif
