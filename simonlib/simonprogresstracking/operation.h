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


#ifndef OPERATION_H
#define OPERATION_H

#include "simonprogresstracking_export.h"
#include <QObject>
#include <QString>
#include <QList>
#include <QPointer>

class QThread;
class Operation;

typedef QPointer<Operation> OperationPtr;
typedef QList<OperationPtr> OperationList;

class SIMONPROGRESSTRACKING_EXPORT Operation : public QObject {
	
	Q_OBJECT

	signals:
		void changed();
		
	public:
		enum Status {
			Running=0,
			Aborting=1,
			Aborted=2,
			Finished=3
		};
		
		Operation(QThread* thread, const QString& name, const QString& currentAction=QString(), int now=0, int max=0, bool isAtomic=true);
		
		void cancel();
		
		void canceled();
		
		void finished();
		
		void update(const QString& currentAction, int newProgress, int newMaximum);
		void update(int newProgress, int newMaximum);
		
		~Operation();
		
		
		//GETTER METHODS
		QThread* thread()
		{
			return m_thread;
		}

		int currentProgress()
		{
			return m_now;
		}
		
		int maxProgress()
		{
			return m_max; 
		}
		
		QString name()
		{
			return m_name;
		}
		
		QString currentAction()
		{
			return m_currentAction;
		}

		bool isAtomic()
		{
			return m_isAtomic;
		}

	private:
		QThread* m_thread;
		QString m_name, m_currentAction;
		int m_now, m_max;
		bool m_cancel, m_isAtomic;
		Status m_status;

};

#endif
