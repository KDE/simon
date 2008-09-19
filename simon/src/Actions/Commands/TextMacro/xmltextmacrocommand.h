#ifndef XMLTEXTMACROCOMMAND_H
#define XMLTEXTMACROCOMMAND_H
#include "../command.h"
#include "../../SimonLib/XML/xmldomreader.h"

/**
 *  @class XMLTextMacroCommand
 *  @brief Extends the XMLDomReader class to handle the commands
 *
 *  @version 0.1
 *  @date 20.05.2008
 *  @author Peter Grasch
 */

class XMLTextMacroCommand : public XMLDomReader {
	
public:
	XMLTextMacroCommand(QString path="conf/commands.xml");
	
	bool save(CommandList *list, QString path="conf/commands.xml");
	
	CommandList* load(bool &ok, QString path="");
	
	~XMLTextMacroCommand();
};


#endif
