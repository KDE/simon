#include "xmlreader.h"
#include "logger.h"


/**
 *  \brief Constructor
 *  \author Martin Gigerl
 */
XMLReader::XMLReader(QString path, QObject *parent) : QObject(parent)
{
	Logger::log(tr("[INF] Initialisiere XML Datei \"%1\"").arg(path));
	this->path=path;
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLReader::~XMLReader()
{
}
