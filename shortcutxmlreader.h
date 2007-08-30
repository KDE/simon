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
#ifndef SHORTCUTXMLREADER_H
#define SHORTCUTXMLREADER_H

#include "xmldomreader.h"
#include "shortcut.h"


/**
 * \class ShortcutXMLReader
 * \brief Reads the shortcuts from the xml file
 * \author Peter Grasch
 * \version 0.1
 * \date 28.8.2007
 */
class ShortcutXMLReader : public XMLDomReader
{
private:
	ShortcutList* shortcuts;
public:
    ShortcutXMLReader(QString path="conf/shortcuts.xml", QObject* parent=0);

    bool load(QString path="");
    ShortcutList* getShortcuts();

    void setShortcuts(ShortcutList *shortcuts);

    bool save(QString path="");

    ~ShortcutXMLReader() { }

};

#endif
