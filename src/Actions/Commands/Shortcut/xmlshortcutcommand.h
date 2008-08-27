//
// C++ Interface: shortcutxmlreader
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef XMLSHORTCUTCOMMAND_H
#define XMLSHORTCUTCOMMAND_H

#include "../../../SimonLib/XML/xmldomreader.h"
#include "../commandmanager.h"
#include "../command.h"


/**
 * \class XMLShortcutCommand
 * \brief Reads the shortcuts from the xml file
 * \author Peter Grasch
 * \version 0.1
 * \date 28.8.2007
 */
class XMLShortcutCommand : public XMLDomReader
{
public:
    XMLShortcutCommand(const QString& path="conf/shortcuts.xml", QObject* parent=0);

    CommandList* load(bool &ok, const QString& path="");

    bool save(const CommandList *commands, const QString& path="");

    ~XMLShortcutCommand();

};

#endif
