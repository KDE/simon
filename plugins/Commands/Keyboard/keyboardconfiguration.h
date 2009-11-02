/*
 *   Copyright (C) 2009 Dominik Neumeister <neudob06@edvhtl.at>
 *   Copyright (C) 2009 Grasch Peter <grasch@simon-listens.org>
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

#ifndef KEYBOARDCONFIGURATION_H
#define KEYBOARDCONFIGURATION_H

#include "keyboardset.h"
#include <simonscenarios/commandconfiguration.h>
#include "ui_keyboardconfigurationdlg.h"
#include <KSharedConfig>
#include <QPointer>
#include <QPoint>
#include <QSize>

class KeyboardSetContainer;
class KeyboardCommandManager;


class KeyboardConfiguration : public CommandConfiguration
{
	Q_OBJECT

	private:
		QPoint m_keyboardPosition;
		QSize m_keyboardSize;

		KeyboardSet *storedSet;
		Ui::KeyboardConfigurationDlg ui;
		KeyboardCommandManager *commandManager;
		KeyboardSetContainer *setContainer;

        private slots:
                void addSet();
		void deleteSet();
		void addTab();
		void deleteTab();
		void addButton();
		void deleteButton();
		void tabUp();
		void tabDown();
		void buttonUp();
		void buttonDown();
                void cbSetsIndexChanged();
                void refreshCbSets();
                void refreshCbTabs();
                void refreshTabDetail();
 
	public slots:
		virtual bool deSerialize(const QDomElement&);
		virtual QDomElement serialize(QDomDocument *doc);
		virtual void defaults();
	
	public:
		KeyboardConfiguration(KeyboardCommandManager* _commandManager, Scenario *parent, const QVariantList &args = QVariantList());
		~KeyboardConfiguration();

		KeyboardSet *getStoredKeyboardSet() { return storedSet; }
		
		void destroy();
		
		//configuration options
		bool caseSensitive();

		QPoint keyboardPosition();
		QSize keyboardSize();
		void saveKeyboardGeometry(const QPoint& position, const QSize& size);

		bool showNumpad();
		QString numberBackspaceTrigger();
		bool enableNumberBasedSelection();
		QString numberBasedSelectionTrigger();
		bool enableNumberWriteOut();
		QString numberWriteOutTrigger();

		bool shift();
		QString shiftTrigger();
		bool capsLock();
		QString capsLockTrigger();
		bool backspace();
		QString backspaceTrigger();
		bool control();
		QString controlTrigger();
		bool returnKey();
		QString returnKeyTrigger();

		bool alt();
		QString altTrigger();
		bool altGr();
		QString altGrTrigger();
		bool super();
		QString superTrigger();
};

#endif
