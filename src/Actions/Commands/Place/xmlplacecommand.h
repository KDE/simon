#ifndef XMLPLACECOMMAND_H
#define XMLPLACECOMMAND_H
#include "../command.h"
#include "../../SimonLib/XML/xmldomreader.h"

/**
 *  @class XMLPlaceCommand
 *  @brief Extends the XMLDomReader class to handle the commands
 *
 *  @version 0.1
 *  @date 20.05.2008
 *  @author Peter Grasch
 */

class XMLPlaceCommand : public XMLDomReader {
	
public:
	XMLPlaceCommand(const QString& path="conf/commands.xml");
	
	bool save(const CommandList *list, const QString& path="conf/commands.xml");
	
	CommandList* load(bool &ok, const QString& path="");
	
	~XMLPlaceCommand();
};


#endif
