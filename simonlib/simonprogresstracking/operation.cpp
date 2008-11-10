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
#include "operation.h"
#include "statusmanager.h"


Operation::Operation(QThread* thread, const QString& name, const QString& currentAction, int now, int max, bool isAtomic) : QObject(0)
{
	m_thread = thread;
	m_name = name;
	m_currentAction = currentAction;
	m_now = now;
	m_max = max;
	m_status = Operation::Running;
	m_isAtomic = isAtomic;
	
	StatusManager::global()->registerOperation(this);
}


void Operation::update(const QString& currentAction, int newProgress, int newMaximum)
{
	m_currentAction = currentAction;
	m_now = newProgress;
	m_max = newMaximum;
	emit changed();
}


void Operation::update(int newProgress, int newMaximum)
{
	m_now = newProgress;
	m_max = newMaximum;
	emit changed();
}

void Operation::cancel()
{
	m_cancel=true;
	m_status = Aborting;
	emit changed();
}

void Operation::canceled()
{
	m_status = Aborted;
	update(maxProgress(), maxProgress());
	emit changed();
}

#include <KDebug>
void Operation::finished()
{
	m_status = Finished;
	update(maxProgress(), maxProgress());
	kDebug() << "Hier";
	emit changed();
}

Operation::~Operation()
{
	StatusManager::global()->removeOperation(this);
}
