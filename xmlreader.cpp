#include "xmlreader.h"
#include "logger.h"


/**
 *  \brief Constructor
 *  \author Martin Gigerl
 */
XMLReader::XMLReader(QString path, QObject *parent) : QObject(parent)
{
	Logger::log("Initializing XML file \""+path+"\"");
	this->path=path;
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLReader::~XMLReader()
{
}
