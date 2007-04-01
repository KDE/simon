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

/**
 *  @class XMLCommand
 *  @brief To handle with xml-commandfiles
 *
 *  @version 0.1
 *  @date 17.03.2007
 *  @author Christoph Kirschner
 *  @todo implementing
 */


typedef QList<Command*> CommandList;


class XMLCommand{

private:
	CommandList commandlist;
	QDomDocument doc;
public:
	XMLCommand(QString path);
	
	void save(CommandList commandlist, QString path);
	
	void load();
	
	CommandList getCommands();
	
	Command getCommand(int index);
	
	~XMLCommand();
};


#endif