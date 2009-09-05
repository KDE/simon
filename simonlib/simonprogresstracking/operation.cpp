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
#include <QTimer>

#include <KLocalizedString>

Operation::Operation(QThread* thread, const QString& name, const QString& currentAction, int now, int max, bool isAtomic) : QObject(0),
	m_thread(thread),
	m_name(name),
	m_currentAction(currentAction),
	m_now(now),
	m_max(max),
	m_isAtomic(isAtomic),
	m_status(Operation::Running)
{
	registerWith(StatusManager::global());
}


void Operation::registerWith(StatusManager *man)
{
	manager << man;
	man->registerOperation(this);
}

QString Operation::currentAction()
{
	switch (m_status)
	{
		case Finished:
			return i18n("Finished");
		case Aborted:
			return i18n("Aborted");
		case Aborting:
			return i18n("Aborting...");
		default:
			return m_currentAction;
	}
}

void Operation::update(const QString& currentAction, int newProgress, int newMaximum)
{
	m_currentAction = currentAction;
	m_now = newProgress;

	if (newMaximum != -1)
		m_max = newMaximum;
	
	pushUpdate();
}



void Operation::update(int newProgress, int newMaximum)
{
	m_now = newProgress;
	if (newMaximum != -1)
		m_max = newMaximum;
	pushUpdate();
}

void Operation::cancel()
{
	m_cancel=true;
	m_status = Aborting;
	pushUpdate();
}

void Operation::canceled()
{
	m_status = Aborted;
	update(maxProgress(), maxProgress());
	pushUpdate();
	QTimer::singleShot(3000, this, SLOT(deleteLater()));
}

void Operation::finished()
{
	m_status = Finished;

	if (maxProgress() > 0)
		update(maxProgress(), maxProgress());
	else update(1,1);

	pushUpdate();
	QTimer::singleShot(3000, this, SLOT(deleteLater()));
}

void Operation::pushUpdate()
{
	foreach (StatusManager *man, manager)
		man->update();
}

Operation::~Operation()
{
	foreach (StatusManager *man, manager)
		man->removeOperation(this);
}
