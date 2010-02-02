/*
 *   Copyright (C) 2008 Phillip Goriup <goriup@simon-listens.org>
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



#ifndef SIMON_SIMONLISTWIDGET_H_B252B690AAEE4A4D883FF6DB45979D2F
#define SIMON_SIMONLISTWIDGET_H_B252B690AAEE4A4D883FF6DB45979D2F

#include <QLineEdit>
#include <KListWidget>
#include "speechgui_export.h"

class SPEECHGUI_EXPORT SimonListWidget : public KListWidget
{	
	
	Q_OBJECT

	/**
	 * \brief Define the current text as a property for use in wizards
	 * \author Peter Grasch
	*/
	Q_PROPERTY(QString currentText READ currentText)
	Q_PROPERTY(QVariant currentUserData READ currentUserData)

private:
	
	QLineEdit *line;
	bool redFlag;
	
public:
	SimonListWidget( QWidget * parent  = 0);
	~SimonListWidget();
	
	void resizeEvent(QResizeEvent * event);
	void keyPressEvent ( QKeyEvent * event );
	void controlRedFlag();
	void showAllEntries();

	/**
	 * \brief Quick access function to retrieve the currently selected text
	 * \author Peter Grasch
	 * @return The currently selected text (or a new QString object) if there is none selected
	 */
	QString currentText() { 
		QListWidgetItem *selected = currentItem();
		if (!selected) return QString();
		return currentItem()->text();
	}

public slots:
	QVariant currentUserData()
	{
		if (!currentItem()) return QVariant();
		return currentItem()->data(Qt::UserRole);
	}
	void showLineEdit();
	void filterEntries(QString text);
	void moveLineEdit();
	void selectItem();
	
	
};


#endif
