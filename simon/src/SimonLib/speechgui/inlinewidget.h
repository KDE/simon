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


#ifndef INLINEWIDGET_H
#define INLINEWIDGET_H

#include <QWidget>
#include <KIcon>

#include "simonwidget.h"
#include "speechgui_export.h"

class QString;
class KIcon;
class QSettings;

/**
 * \class InlineWidget
 * \brief Provides a widget to display inline in the simon mainwindow
 * \note Please use this instead of the QDialog class whenever feasable!
 * \author Peter Grasch
 * \version 0.1
 * \date 10.8.2007
*/
class SPEECHGUI_EXPORT InlineWidget : public QWidget, public SimonWidget
{
Q_OBJECT

private:
	QString title, desc;
	KIcon icon;
	GuiEvents *guievents;

signals:
	void closed();
	void rejected();
	void hidden();
	void execd();
	void shown();
	void accepted();

public:
	InlineWidget(QString title, KIcon icon, QString desc, QWidget* parent=0);
		
	~InlineWidget();

	void setVisible(bool visible);

	QString getTitle() const {return title; }
	KIcon getIcon() const { return icon; }
	QString getDesc() const { return desc; }
	bool isShown();

public slots:
	void accept();
	void reject();
	bool close();
	bool exec();

	void registerControl(QString trigger, QObject* receiver, const char* slot);
	void doAction(QString action);

	QObjectList getChildren(QObject *current);

};

#endif
