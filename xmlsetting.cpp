#include "xmlsetting.h"


XMLSetting::XMLSetting(QString path):XMLReader(path)
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
XMLSetting::~XMLSetting()
{
                         
}

