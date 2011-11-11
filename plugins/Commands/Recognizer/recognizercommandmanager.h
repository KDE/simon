/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_RECOGNIZERCOMMANDMANAGER_H_19E629596C1B44AF866AF7901F4DD37B
#define SIMON_RECOGNIZERCOMMANDMANAGER_H_19E629596C1B44AF866AF7901F4DD37B

#include <simonscenarios/commandmanager.h>
#include <QVariantList>

/**
 *	@class RecognizerCommandManager
 *	@brief Manager for the recognizer
 *
 *	@version 0.1
 *	@date 16.10.2008
 *	@author Peter Grasch
 */
class RecognizerCommandManager : public CommandManager
{
  Q_OBJECT

    protected:
    bool trigger(const QString& triggerName);

  public:
    const QString preferredTrigger() const { return ""; }
    const QString name() const;
    const QString iconSrc() const;
    bool deSerializeConfig(const QDomElement& elem);

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    RecognizerCommandManager(QObject* parent, const QVariantList& args);

    ~RecognizerCommandManager();

};
#endif
