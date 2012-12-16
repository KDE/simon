/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
 *
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License version 2,
 *   or (at your option) any later version, as published by the Free
 *   Software Foundation
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details
 *
 *   You should have received a copy of the GNU General Public
 *   License along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_AICOMMANDMANAGER_H_6DBF1A00634B4F5BB3581C867E9B723C
#define SIMON_AICOMMANDMANAGER_H_6DBF1A00634B4F5BB3581C867E9B723C

#include <simonscenarios/commandmanager.h>

#include <QVariantList>

/**
 *	@class AICommandManager
 *	@brief Manager of the ScreenGrid
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class AIMLParser;
class KProcess;

class AICommandManager : public CommandManager
{
  Q_OBJECT

    private:
    AIMLParser *parser;

  protected:
    bool trigger(const QString& triggerName, bool silent);

  public:
    bool deSerializeConfig(const QDomElement& elem);
    const QString name() const;
    const QString iconSrc() const;
    bool setupParser();
    bool addCommandPrivate(Command *) { return false; }

    CommandConfiguration* getConfigurationPage() const;
    
    void finalize();

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    AICommandManager(QObject* parentScenario, const QVariantList& args);

    ~AICommandManager();

};
#endif
