#ifndef XMLEXECUTABLECOMMAND_H
#define XMLEXECUTABLECOMMAND_H
#include "../command.h"
#include "../../SimonLib/XML/xmldomreader.h"

/**
 *  @class XMLExecutableCommand
 *  @brief Extends the XMLDomReader class to handle the commands
 *
 *  @version 0.1
 *  @date 20.05.2008
 *  @author Peter Grasch
 */

class XMLExecutableCommand : public XMLDomReader {
	
public:
	XMLExecutableCommand(const QString& path="conf/commands.xml");
	
	bool save(CommandList *list, const QString& path="");
	
	CommandList* load(bool &ok, const QString& path);
	
	~XMLExecutableCommand();
};


#endif
