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
 
#ifndef PROGRESSWIDGET_H
#define PROGRESSWIDGET_H

#include <QWidget>
#include <QPointer>

class Operation;
class QLabel;
class QPushButton;
class QProgressBar;
class KPushButton;

/**
	@author 
*/
class ProgressWidget : public QWidget
{
Q_OBJECT
	private:
		Operation *op;
		QLabel *name;
		QLabel *currentAction;
		QProgressBar *bar;
		KPushButton *cancelButton;
	
	public slots:
		void update();

	public:
		ProgressWidget(QPointer<Operation> op, QWidget* parent=0);
		
		QPointer<Operation> operation() { return op; }

		~ProgressWidget();

};

#endif
