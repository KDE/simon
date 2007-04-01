#ifndef XMLSETTING_H
#define XMLSETTING_H
#include "xmlreader.h"
#include "setting.h"
/**
 *  @class XMLSetting
 *  @brief To handle with xml-settingdocuments
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 *  @todo implementing
 */


typedef QList<Setting*> SettingList;

class XMLSetting: public XMLReader
{
private:
	SettingList settinglist;
public:
	XMLSetting(QString path);
	
	void safe(SettingList settinglist, QString path);
	
	void load();
	
	SettingList getSettings();
	
	Setting getSetting(int index);
	
	~XMLSetting();
};

#endif