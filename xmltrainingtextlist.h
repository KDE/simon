//
// C++ Interface: xmltrainingtextlist
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef XMLTRAININGTEXTLIST_H
#define XMLTRAININGTEXTLIST_H

#include <QString>
#include <QHash>
#include "xmldomreader.h"

/**
 * \class XMLTrainingTextList
 * \brief This class handles the list of avalible Traininngtexts when importing from the net
 * \author Peter Grasch
 * \version 0.1
 * \date 3.06.07
*/
class XMLTrainingTextList : public XMLDomReader {
Q_OBJECT
private:
	QHash<QString,QString> trainingtexts;
public:
    XMLTrainingTextList(QString path);
    bool load(QString path="");
    
    QHash<QString,QString> getTrainingTextList() { return trainingtexts; }

    ~XMLTrainingTextList();

};

#endif
