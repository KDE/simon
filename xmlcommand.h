#ifndef XMLCOMMAND_H
#define XMLCOMMAND_H
#include <QtXml>
#include <QString>
#include <QList>
#include <QFile>
#include <QIODevice>
#include <QStringList>
#include <QDomNode>
#include <QDomDocument>
#include "command.h"
#include "xmldomreader.h"

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
	
	void save(QString path="conf/commands.xml");

	void replaceCommand(QString commandName, Command *newCommand);
	void addCommand(Command *newCommand);
	void deleteCommand(QString commandName);
	bool commandExists(QString commandName);
	
	void load(QString path="");
	
	CommandList getCommands();
	
	~XMLCommand();
};


#endif
