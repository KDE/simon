/*
 *   Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_MANAGEACTIONSDIALOG_H_FFF7A6377C9C4E42B2B5636676AD423F
#define SIMON_MANAGEACTIONSDIALOG_H_FFF7A6377C9C4E42B2B5636676AD423F


#include <KDialog>
#include <QHash>
#include <QWidget>
#include <QVBoxLayout>
#include "ui_manageactionsdlg.h"
class Action;
class CommandConfiguration;
class KPageWidget;
class KPageWidgetItem;
class ListConfiguration;

class ManageActionsDialog : public KDialog {

Q_OBJECT

private:
	Ui::DlgManageActions ui;
	KPageWidget *pageWidget;

	QHash<CommandConfiguration*, KPageWidgetItem*> pages;

	QList<CommandConfiguration*>* configurationPages;

	ListConfiguration *listConfiguration;


	Action* getCurrentlySelectedAction();
	void registerCommandConfiguration(CommandConfiguration *m);

private slots:
	void applyTrigger(const QString&);
	void applyTriggerToAll();
	void add();
	void remove();
	void moveUp();
	void moveDown();
	void currentSelectionChanged();

public:
	int exec();
	ManageActionsDialog(QWidget *parent=0);
	~ManageActionsDialog();
};

class ProtectorWidget : public QWidget {
	private:
		QWidget* m_widget;

	public:
		ProtectorWidget(QWidget *w, QWidget *parent) : QWidget(parent), m_widget(w)
		{
			QVBoxLayout *lay = new QVBoxLayout(this);
			lay->addWidget(m_widget);
		}

		~ProtectorWidget() 
		{
			//don't delete m_widget
			m_widget->setParent(NULL);
		}
};


#endif

