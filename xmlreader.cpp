#include "xmlreader.h"


/**
 *  \brief Constructor
 *  \author Martin Gigerl
 */
XMLReader::XMLReader(QString path, QObject *parent) : QObject(parent)
{
    this->path=path;
}



/**
 * \brief Destructor
 * \author Peter Grasch
 */
XMLReader::~XMLReader()
{
}
