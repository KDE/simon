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

#ifndef SIMONTABLEWIDGET_H
#define SIMONTABLEWIDGET_H

#include <QTableWidget>
#include <QVector>
#include "simonlineedit.h"
#include "speechgui_export.h"

class QLineEdit;


class SPEECHGUI_EXPORT SimonTableWidget : public QTableWidget
{	
	
	Q_OBJECT
	
private:
	
	bool redFlag;
	QVector<SimonLineEdit*> linevector;
	int count;

public:
	SimonTableWidget( QWidget * parent  = 0);
	~SimonTableWidget();
	
	void resizeEvent(QResizeEvent * event);
	void controlRedFlag(int lineValue);
	void showEntries();
	void fillVector();
	void invisibleRows(SimonLineEdit * temp, QString text);
	void returnFilterEntries(SimonLineEdit* temp);
	SimonLineEdit* getFocusedLineEdit();
	void invisibleLineEdits();
	QTableWidgetItem* selectItem();
	

public slots:
	void showLineEdit();
	void filterEntries(QString text);
	void initLineEdit(QString action);
	void checkFocus();
	void moveLineEdit();

	

protected:
	virtual void keyPressEvent(QKeyEvent *e);
	virtual void focusOutEvent ( QFocusEvent * event );

signals:
    void returnPressed();
	
	
};

#endif

