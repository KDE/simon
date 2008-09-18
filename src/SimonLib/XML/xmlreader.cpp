#include "xmlreader.h"
#include "../Logging/logger.h"
#include <KLocalizedString>


/**
 *  \brief Constructor
 *  \author Martin Gigerl
 */
XMLReader::XMLReader(QString path, QObject *parent) : QObject(parent)
{
	Logger::log(i18n("[INF] Initialisiere XML Datei \"%1\"", path));
	this->path=path;
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLReader::~XMLReader()
{
}
