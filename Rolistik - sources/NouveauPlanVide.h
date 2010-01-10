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
/* Fenetre destinee a parametrer un nouveau plan vide avant sa      */
/* creation                                                         */
/*                                                                  */
/********************************************************************/	


#ifndef NOUVEAU_PLAN_VIDE_H
#define NOUVEAU_PLAN_VIDE_H

    #include <QDialog>
    #include <QLineEdit>
    #include <QPushButton>
    #include <QRadioButton>
	#include <QGroupBox>


    class NouveauPlanVide : public QDialog
    {
	Q_OBJECT

    public :
	    NouveauPlanVide(QWidget *parent = 0);

	signals :
		void creerNouveauPlanVide(QString titre, QString idCarte, QColor couleurFond, quint16 largeur, quint16 hauteur, quint8 taillePj);

	private slots :
		void ouvrirSelecteurCouleur();
		void changementFormat(int bouton);
		void changementDimensions(int bouton);
		void validerDimensions();

    private :
		void majIntitulesBoutons();

		QGroupBox *groupeFormat;
		QGroupBox *groupeTaille;
		QLineEdit *titrePlan;
		QLineEdit *largeurPlan;
		QLineEdit *hauteurPlan;
		QPushButton *couleurFond;
		QRadioButton *boutonPetitPlan;
		QRadioButton *boutonMoyenPlan;
		QRadioButton *boutonGrandPlan;
		QRadioButton *boutonTresGrandPlan;
		QRadioButton *boutonPersonnalise;
		QWidget *taillePersonnalisee;
		int format;							// Indice dans les tableaux ci-dessous correspondant au format selectionne
		int dimensions;						// Indice dans les tableaux ci-dessous correspondant aux dimensions selectionnees

		// Tableaux contenant les dimensions du plan au format [format] [dimension]
		static quint16 largeur[3][4];
		static quint16 hauteur[3][4];

	};

#endif
