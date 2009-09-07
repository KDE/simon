/*
 *   Copyright (C) 2009 Dominik Neumeister <neudob06@edvhtl.at>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "keyboardsetxmlreader.h"

#include <QString>
#include <QList>
#include <QFile>
#include <QStringList>
#include <QDomNode>
#include <QDomDocument>
#include <KFilterDev>
#include <QDebug>
#include <QDir>


KeyboardsetXMLReader::KeyboardsetXMLReader(const QString& path):XMLDomReader(path)
{ }

bool KeyboardsetXMLReader::save(QList<KeyboardSet *> * setList, const QString &path)
{
        if (this->doc)
                this->doc->clear();
        else doc = new QDomDocument ();

        QDomElement root = doc->createElement("keyboardsets");
        doc->appendChild(root);

        for (int i=0; i < setList->size(); i++)
        {
                KeyboardSet *kbs = dynamic_cast<KeyboardSet*>(setList->at(i));
                if (!kbs) continue;

                QDomElement set = doc->createElement("set");
                QDomElement setname = doc->createElement("setname");
                setname.appendChild(doc->createTextNode(kbs->getSetName()));
                set.appendChild(setname);

                for(int j=0; j < kbs->getTabList()->size(); j++)
                {
                    KeyboardTab *kbt = dynamic_cast<KeyboardTab*>(kbs->getTabList()->at(j));
                    if (!kbt) continue;

                    QDomElement tab = doc->createElement("tab");
                    QDomElement tabname = doc->createElement("tabname");
                    tabname.appendChild(doc->createTextNode(kbt->getTabName()));
                    tab.appendChild(tabname);

                    for(int k=0; k < kbt->getButtonList()->size(); k++)
                    {
                        KeyboardButton *kbb = dynamic_cast<KeyboardButton*>(kbt->getButtonList()->at(k));
                        if (!kbb) continue;

                        QDomElement button= doc->createElement("button");
                        QDomElement triggershown = doc->createElement("triggershown");
                        triggershown.appendChild(doc->createTextNode(kbb->getTriggerShown()));
                        QDomElement realtrigger = doc->createElement("realtrigger");
                        realtrigger.appendChild(doc->createTextNode(kbb->getTriggerReal()));
                        QDomElement valuetype = doc->createElement("valuetype");
                        valuetype.appendChild(doc->createTextNode(QString::number(kbb->getValueType())));
                        QDomElement value = doc->createElement("value");
                        value.appendChild(doc->createTextNode(kbb->getValue()));

                        button.appendChild(triggershown);
                        button.appendChild(realtrigger);
                        button.appendChild(valuetype);
                        button.appendChild(value);

                        tab.appendChild(button);
                    }
                    set.appendChild(tab);
                }
                root.appendChild(set);
        }

        //return XMLDomReader::save(path);

        QIODevice *file = KFilterDev::deviceForFile("/home/domar/domarstest.xml","text/plain");
qDebug() << "nach QIODevice";
        if(!file->open(QIODevice::WriteOnly))
        {
qDebug() << "in da ersten if" << file->errorString();
            return false;
        }
qDebug() << "vor QTextStream";
        QTextStream ts(file);
        qDebug() << "nach QTextStream";
        ts.setCodec("UTF-8");
        ts << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
        ts << doc->toString();
        qDebug() << "vor emit(written)";
        emit(written());
        file->close();
qDebug() << "saved!" << path << doc;
        return true;
}

QList<KeyboardSet *> * KeyboardsetXMLReader::load(QString path)
{
        if(doc)
            delete doc;
        doc= new QDomDocument();

        QIODevice *file = KFilterDev::deviceForFile("/home/domar/domarstest.xml", "text/plain");

        if(!file->open(QIODevice::ReadOnly))
                return false;

        if (!doc->setContent(file))
                return false;

        file->close();
        file->deleteLater();

        emit (loaded());

        QList<KeyboardSet *> *setList = new QList<KeyboardSet *>();

        QDomElement root = this->doc->documentElement();
        if(!root.isNull())
        {
            QDomElement set = root.firstChildElement();
            QDomElement setname = set.firstChildElement();
            QList<KeyboardTab *> *tabList = new QList<KeyboardTab *>();
            while(!set.isNull())
            {
                QDomElement tab = set.nextSiblingElement();
                QDomElement tabname = tab.firstChildElement();
                QList<KeyboardButton *> *buttonList = new QList<KeyboardButton *>();
                while(!tab.isNull())
                {
                    QDomElement button = tab.nextSiblingElement();
                    QDomElement triggershown = button.firstChildElement();
                    QDomElement realtrigger = button.nextSiblingElement();
                    QDomElement valuetype = button.nextSiblingElement();
                    QDomElement value = button.nextSiblingElement();

                    buttonList->append(new KeyboardButton(triggershown.text(), realtrigger.text(), valuetype.text().toShort(), value.text()));
                }
                tabList->append(new KeyboardTab(tabname.text(), buttonList));//TODO: write weiter here!
            }
            setList->append(new KeyboardSet(setname.text(), tabList));

            while(!root.isNull())
            {
                set = root.nextSiblingElement();
                setname = set.firstChildElement();
                QList<KeyboardTab *> *tabList = new QList<KeyboardTab *>();
                while(!set.isNull())
                {
                    QDomElement tab = set.nextSiblingElement();
                    QDomElement tabname = tab.firstChildElement();
                    QList<KeyboardButton *> *buttonList = new QList<KeyboardButton *>();

                    while(!tab.isNull())
                    {
                        QDomElement button = tab.nextSiblingElement();
                        QDomElement triggershown = button.firstChildElement();
                        QDomElement realtrigger = button.nextSiblingElement();
                        QDomElement valuetype = button.nextSiblingElement();
                        QDomElement value = button.nextSiblingElement();

                        buttonList->append(new KeyboardButton(triggershown.text(), realtrigger.text(), valuetype.text().toShort(), value.text()));
                    }
                    tabList->append(new KeyboardTab(tabname.text(), buttonList));//TODO: write weiter here!
                }
                setList->append(new KeyboardSet(setname.text(), tabList));
            }
        }

/*        QDomElement root = this->doc->documentElement();
        QDomElement set = root.firstChildElement();

        while(!root.isNull())
        {
                QList<KeyboardTab *> *tabList = new QList<KeyboardTab *>();
                QDomElement setname = set.firstChildElement();
                QDomElement tab = set.nextSiblingElement();

                while(!set.isNull())
                {
                    QList<KeyboardButton *> *buttonList = new QList<KeyboardButton *>();
                    QDomElement tabname = tab.firstChildElement();
                    QDomElement button = tab.nextSiblingElement();

                    while(!tab.isNull())
                    {
                        QDomElement triggershown = button.firstChildElement();
                        QDomElement realtrigger = button.nextSiblingElement();
                        QDomElement valuetype = button.nextSiblingElement();
                        QDomElement value = button.nextSiblingElement();

                        buttonList->append(new KeyboardButton(triggershown.text(), realtrigger.text(), valuetype.text().toShort(), value.text()));
                        button = button.nextSiblingElement();
                    }
                    tabList->append(new KeyboardTab(tabname.text(), buttonList));
                    tab = tab.nextSiblingElement();
                }
                setList->append(new KeyboardSet(setname.text(), tabList));
                set = set.nextSiblingElement();
        }*/
        return setList;
}

KeyboardsetXMLReader::~KeyboardsetXMLReader()
{

}
