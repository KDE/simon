#include "xmlsetting.h"




XMLSetting::XMLSetting(QString path):XMLReader(path)
{
	//XMLReader reader(path);
}


void XMLSetting::safe(SettingList settinglist, QString path)
{
	QDomDocument doc("mysettings");
	QDomElement root = doc.createElement("settinglist");
	doc.appendChild(root);
	
	QDomNode newnode;		

	for(int i=0; i<=settinglist.size(); i++)
	{
		newnode.setNodeValue(settinglist[i]->getValue());

		QDomElement setting = newnode.toElement();
		setting.setTagName("setting");

		setting.setAttribute("name", settinglist[i]->getName());
	
		root.appendChild(setting);		
	}
}


void XMLSetting::load()
{
	QDomElement root = this->doc.documentElement();
	
	QDomNode n = root.firstChild();
	QDomElement setting = n.toElement();
	
	while(!setting.isNull())
	{
		QString name = setting.attribute("name");
		QString hardware = setting.attribute("hardware");
		QString value = setting.text();
						
		settinglist.append(new Setting(name, value, hardware));

		setting = n.nextSiblingElement();
	}
}


SettingList XMLSetting::getSettings()
{
	return this->settinglist;
}


Setting XMLSetting::getSetting(int index)
{
	return *(settinglist[index]);
}


XMLSetting::~XMLSetting()
{
}
