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
	XMLPlaceCommand(QString path="conf/commands.xml");
	
	bool save(CommandList *list, QString path="conf/commands.xml");
	
	CommandList* load(bool &ok, QString path="");
	
	~XMLPlaceCommand();
};


#endif
