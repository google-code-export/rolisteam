/***************************************************************************
 *   Copyright (C) 2011 by Renaud Guezennec                                *
 *   http://renaudguezennec.homelinux.org/accueil,3.html                   *
 *                                                                         *
 *   Rolisteam is free software; you can redistribute it and/or modify     *
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
#include <QObject>
#include <QtCore/QString>
#include <QtTest/QtTest>
#include <data/chapter.h>

class TestData : public QObject
{
    Q_OBJECT

public:
    TestData(QObject *parent=NULL);

private Q_SLOTS:
    void initTestCase();
    void cleanupTestCase();
    void testAddChapter();
    void testAddChapterHasChildren();
    void testSetGetName();
private:
    Chapter* m_chapter;
};


TestData::TestData(QObject *parent) :
    QObject(parent)
{
}

void TestData::initTestCase()
{
    m_chapter = new Chapter();
}

void TestData::cleanupTestCase()
{
    delete m_chapter;

}
void TestData::testAddChapterHasChildren()
{
    for(int i = 0; i< 2000; i++)
    {
        QString temp("Chapiter %1");
        temp=temp.arg(i);
        RessourcesNode* item = m_chapter->addChapter(temp);
    }
    QVERIFY2(m_chapter->hasChildren(),"No Children");

}
void TestData::testSetGetName()
{
    for(int i = 0; i< 2000; i++)
    {
        QString temp("Chapiter %1");
        temp=temp.arg(i);
        m_chapter->setShortName(temp);
        QVERIFY2(m_chapter->getShortName()==temp,"Name is not as expected!");
    }

}

void TestData::testAddChapter()
{
    for(int i = 0; i< 2000; i++)
    {
        QString temp("Chapiter %1");
        temp=temp.arg(i);
        RessourcesNode* item = m_chapter->addChapter(temp);
        QVERIFY2(m_chapter->removeRessourcesNode(item),"Removal fails, the item was not in the list");
    }
    QVERIFY2(!m_chapter->hasChildren(),"all Children have not been removed");

}

QTEST_MAIN(TestData);

#include "testdata.moc"
