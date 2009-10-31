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

#ifndef FILTERCOMMANDMANAGER_H
#define FILTERCOMMANDMANAGER_H

#include <simonscenarios/commandmanager.h>

#include <QVariantList>

class KAction;

/**
 *	@class FilterCommandManager
 *	@brief Manager of the Filter
 *
 *	@version 0.1
 *	@date 22.08.2009
 *	@author Peter Grasch
 */
class FilterCommandManager : public CommandManager {
Q_OBJECT

protected:
	bool isActive;
	bool trigger(const QString& triggerName);
	void updateAction();

public slots:
	void toggle();
public:
	const KIcon icon() const;

	const QString name() const;
	bool deSerializeConfig(const QDomElement& elem, Scenario *parent);
	bool addCommand(Command *) { return false; }


	CommandConfiguration* getConfigurationPage();

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    FilterCommandManager(QObject* parent, const QVariantList& args);

    
    ~FilterCommandManager();

private:
    KAction *activateAction;

};

#endif
