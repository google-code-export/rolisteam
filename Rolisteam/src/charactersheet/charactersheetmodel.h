/***************************************************************************
 *	 Copyright (C) 2009 by Renaud Guezennec                                *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
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

#ifndef CHARACTERSHEETMODEL_H
#define CHARACTERSHEETMODEL_H

#include <QAbstractItemModel>

#include <QTextStream>
#include <QFile>

class CharacterSheet;
class Section;

/**
  * @brief stores section and represent it into the tree model.
  */
class TreeItem
{
public:
    /**
      * @brief constructor with parameter
      * @param Section pointer
      * @param isLeaf either the item is a leaf or not.
      */
    TreeItem(Section* p,bool isLeaf);

    /**
      * @brief defines the stored section
      */
    void setSection(Section* p);
    /**
      * @brief accessor to the stored section
      */
    Section* getSection();

    /**
      * @brief return either the item is a leaf or note.
      */
    bool isLeaf();
    /**
      * @brief defines if the item is a leaf or not
      */
    void setLeaf(bool leaf);
    /**
      * @brief get the item's parent
      * @return pointer to its parent
      */
    TreeItem* getParent();
    /**
      * @brief defines the item's parent
      * @param new parent
      */
    void setParent(TreeItem* p);
    /**
      * @brief adds child in this item.
      * @param new child
      */
    void addChild(TreeItem* child);
    /**
      * @brief gives the child at the given position
      * @param the position (row) of the wanted item.
      * @return TreeItem's pointer or NULL if row is out of range.
      */
    TreeItem* child(int row);
    /**
      * @brief gives the position of the current item into is father.
      */
    int row();
    /**
      * @brief gives the position of the given child in the children list.
      * @param the child
      * @return position of child, -1 if not matched.
      */
    int indexOfChild(TreeItem* itm);
    /**
      * @brief accessor to the children count.
      */
    int childrenCount();
private:
    Section* m_section;
    bool m_isLeaf;
    TreeItem* m_parent;
    QList<TreeItem*>* m_children;

};


/**
* @brief CharacterSheetModel is part of the MVC architecture for charactersheet viewer. it herits from QAbstractItemModel
* it also provides features for adding data into stored CharacterSheet.  
*/
class CharacterSheetModel : public QAbstractItemModel
{
public:
	/**
	* @brief default constructor
	*/
    CharacterSheetModel();


	/**
	* @brief compulsory function which returns the  row count : should be the higher index count of stored characterSheets 
	* @param parent useless in tablemodel (only relevant with a tree.)
	*/
    int rowCount ( const QModelIndex & parent = QModelIndex() ) const ;
	/**
	* @brief build an index with the given location and parent.
	* It should return invalide index when location is out of data range.
	* @param row : the row coordinate
	* @param column : the column coordinate
	* @param parent : all items in the table should be child of an invalide QModelIndex.
	*/
    QModelIndex index ( int row, int column, const QModelIndex & parent = QModelIndex() ) const  ;
	/**
	* @brief build the parent of given index. Useless in our case.
	*/
    QModelIndex parent ( const QModelIndex & index ) const ;
	/**
	* @brief compulsory function which returns the column count: it is egal to the number of characterSheet stored in the model.
	* @param : parent useless because in a tableview/model all items have the same parent (root).
	*/
    int columnCount ( const QModelIndex & parent = QModelIndex() ) const;
	/**
	* @brief compulsory function which returns the value of the given index.
	* @param index : the location of the wished element
	* @param role : the data role.
	*/
    QVariant data ( const QModelIndex & index, int role = Qt::DisplayRole ) const;
	/**
	* @brief allows editing. The model can modify the data beacause of the function.
	* @param index : location of the amended data.
	* @param value : new value
	* @param role : the data role
	*/
    bool setData ( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole );



	/**
	* @brief adds an empty CharacterSheet into the model.
	*/
    void addCharacterSheet();
	/**
	* @brief adds section after the given Index.
	* @param index location of the new section
	*/
    void addSection( /*int index*/);
	/**
	* @brief adds line at the given index
	* @param index location of the new line. 
	*/
    void addLine(const QModelIndex & index);


    Qt::ItemFlags flags(const QModelIndex &index) const;
    bool hasChildren ( const QModelIndex & parent  ) const;


    QVariant headerData(int section, Qt::Orientation orientation,
                                   int role) const;


    Section* indexToSection(const QModelIndex & index);
    QModelIndex indexToSectionIndex(const QModelIndex & index);



    bool writeModel(QTextStream& file, bool data= true);
    bool readModel(QFile& file);

    TreeItem* addSection(QString title);
    void addLine(TreeItem* parentItem,QString name);

private:
	/**
	* @brief QList which stores pointer to CharacterSheet. 
	*/ 
    QList<CharacterSheet*>* m_characterList;


    /**
    * @brief QList which stores pointer to CharacterSheet.
    */
    QList<Section*>* m_sectionList;


    Section* m_rootSection;

    TreeItem* m_rootItem;
};

#endif // CHARACTERSHEETMODEL_H
