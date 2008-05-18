#ifndef XMLCOMMAND_H
#define XMLCOMMAND_H
#include "command.h"
#include "../SimonLib/XML/xmldomreader.h"

/**
 *  @class XMLCommand
 *  @brief Extends the XMLDomReader class to handle the commands
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Peter Grasch
 */


typedef QList<Command*> CommandList;


class XMLCommand : public XMLDomReader {

private:
	CommandList commandlist;
	
public:
	XMLCommand(QString path="conf/commands.xml");
	
	bool save(QString path="conf/commands.xml");
	
	bool load(QString path="");
	
	CommandList getCommands();
	void setCommands(CommandList commands) 
	{ this->commandlist = commands; }
	
	~XMLCommand();
};


#endif
