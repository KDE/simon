#include "xmlreader.h"

#include <QMessageBox>

/**
 *  @class XMLReader.cpp
 *  
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Martin Gigerl
 *  @todo Implementing
 */
XMLReader::XMLReader(QString path)
{
    this->path=path;
}


int XMLReader::save(QString path)
{
     QFile file(path);
     if(!file.open(QIODevice::WriteOnly ) )
     return -1;
     QTextStream ts(&file);
     ts << doc->toString(); 
     file.close();
     return 0;
}
QDomElement XMLReader::settingToNode(QDomDocument &d, QString name, QString value )
{

   QDomElement c = d.createElement( "option" );
      c.setAttribute( "value", name);
   c.setAttribute( "name", value);

   

   return c;
}

void XMLReader::load(QString path)
{
     if (path.isEmpty()) path = this->path;
        doc= new QDomDocument();
		QFile file(path);
		if(!file.open(QIODevice::ReadOnly))
			return;
		if(!doc->setContent(&file))
		{
			file.close();
			return;
		}
		file.close();

}


XMLReader::~XMLReader()
{
}
