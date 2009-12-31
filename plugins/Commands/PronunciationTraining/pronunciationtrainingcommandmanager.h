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

#ifndef SIMON_PRONUNCIATIONTRAININGCOMMANDMANAGER_H_08C58336D74C4721A047F68738AB6253
#define SIMON_PRONUNCIATIONTRAININGCOMMANDMANAGER_H_08C58336D74C4721A047F68738AB6253

#include <simonscenarios/commandmanager.h>

#include <QVariantList>

class KAction;

/**
 *	@class PronunciationTrainingCommandManager
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class PronunciationTrainingCommandManager : public CommandManager{
Q_OBJECT

protected:
	bool trigger(const QString& triggerName);

public slots:
	void activateTraining();

public:
	const QString preferredTrigger() const;
	const QString name() const;
	const KIcon icon() const;
	bool addCommand(Command *) { return false; }
	bool deSerializeConfig(const QDomElement& elem);

    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    PronunciationTrainingCommandManager(QObject* parent, const QVariantList& args);

    
    ~PronunciationTrainingCommandManager();

private:
    KAction *activateAction;

};

#endif
