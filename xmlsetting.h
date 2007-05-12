#ifndef XMLSETTING_H
#define XMLSETTING_H
#include "xmldomreader.h"
#include <QHash>
#include <QString>
#include <QDomNode>
#include <QDomDocument>
/**
 *  @class XMLSetting
 *  @brief To handle with xml-settingdocuments
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 *  @todo implementing
 */
class XMLSetting: public XMLDomReader
{

public:
	XMLSetting();
    int saveSettings(QHash<QString,QString>* settings);
    QHash<QString,QString>* getSettings();
    QDomElement settingToNode(QDomDocument &d, QString name, QString value );	
    void loadSettings();
	~XMLSetting();
};

#endif

