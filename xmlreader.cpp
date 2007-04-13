#include "xmlreader.h"
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


void XMLReader::save(QString path)
{
     if (path.isEmpty()) path = this->path;
    
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
