#ifndef XMLSETTING_H
#define XMLSETTING_H
#include "xmlreader.h"
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
class XMLSetting: public XMLReader
{

public:
	XMLSetting();
    int saveSettings(QHash<QString,QString>* settings);
    QHash<QString,QString>* getSettings();	
    void loadSettings();
	~XMLSetting();
};

#endif

