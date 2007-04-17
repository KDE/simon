#include "xmlsetting.h"

#include <QMessageBox>



XMLSetting::XMLSetting():XMLReader("conf/setting.xml")
{
	
}

QHash<QString,QString>* XMLSetting::getSettings()
{
    QHash<QString,QString> *settings = new QHash<QString,QString>();
    QDomElement root = doc->documentElement(); 
    
    QDomNode node = root.firstChild();    
    
      while ( !node.isNull() ) { 
          
     
       
         QDomElement ent = node.toElement(); 
                    
            if( (!ent.isNull()) && (ent.tagName() == "option") ) 
            { 
                      settings->insert(ent.attribute("name"), ent.attribute("value"));
            } 
            node = node.nextSibling();
     }
     return settings;
}

int XMLSetting::saveSettings(QHash<QString,QString>* settings)
{
     doc=new QDomDocument();
     QDomElement root = doc->createElement("settings");
     doc->appendChild(root);
     for (int i=0; i<settings->count(); i++)
     {

       root.appendChild(this->settingToNode(*doc,QString(settings->values().at(i)),QString(settings->keys().at(i))));       
     }
     return this->save("conf/settings.xml"); 
}

void XMLSetting::loadSettings()
{
     this->load("conf/settings.xml");    
}


XMLSetting::~XMLSetting()
{
                         
}

