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

#ifndef COMMANDSETTINGSINTERNAL_H
#define COMMANDSETTINGSINTERNAL_H

#include "actionlib_export.h"
#include <simonscenarios/commandlistelements.h>
#include <QFont>
#include <QList>
#include <QHash>
#include <QObject>
#include <KSharedConfig>


class QListWidgetItem;
class KPageWidget;
class KPageWidgetItem;
class Action;
class VoiceInterfaceCommand;
class ListConfiguration;

class SIMONACTIONS_EXPORT CommandSettingsInternal : public QObject
{
	Q_OBJECT
	
	signals:
		void recognitionResultsFilterParametersChanged();

  private:
    bool forceChangeFlag;
    static CommandSettingsInternal* instance;

    KSharedConfig::Ptr config;
		
		bool storedDYM;
		float storedConfidence;
    QFont storedFont;

    ListConfiguration *listConfiguration;
		
	public slots:
		virtual void save(bool dym, float confidence, const QFont& font);
		virtual void load();
		virtual void defaults();

public:
    CommandSettingsInternal(QWidget *parent=0);
    virtual ~CommandSettingsInternal();
		
    static CommandSettingsInternal* getInstance(QWidget *parent=0) {
      if (!instance) return new CommandSettingsInternal(parent);
      return instance;
    }
    
    QHash<CommandListElements::Element, VoiceInterfaceCommand*> getListInterfaceCommands();

    float minimumConfidence();
    bool useDYM();
    QFont pluginBaseFont();
		ListConfiguration* getListConfiguration();
};

#endif // COMMANDSETTINGSINTERNAL_H
