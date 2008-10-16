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

#include "executablecommand.h"
#include <QObject>
#include <QVariant>
#include <KIcon>
#include <KProcess>
#include <KLocalizedString>


const QString ExecutableCommand::staticCategoryText()
{
	return i18n("Programm");
}

const QString ExecutableCommand::getCategoryText() const
{
	return ExecutableCommand::staticCategoryText();
}

const KIcon ExecutableCommand::staticCategoryIcon()
{
	return KIcon("applications-system");
}

const KIcon ExecutableCommand::getCategoryIcon() const
{
	return ExecutableCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> ExecutableCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(i18n("Ausführbare Datei"), getExecutable());
	out.insert(i18n("Arbeitspfad"), getWorkingDirectory());
	return out;
}

bool ExecutableCommand::triggerPrivate()
{
	QStringList commands = exe.split(";");
	for (int i=0; i < commands.count(); i++)
	{
		QString exe = commands[i].trimmed();

		QString executable;
		QStringList args;
		if (exe.contains(" "))
		{
			executable = exe.left(exe.indexOf(" "));
			QString argumentStrs = exe.mid(executable.count());
			args = argumentStrs.split(" ", QString::SkipEmptyParts);
		} else executable = exe;

		KProcess proc;
		proc.setWorkingDirectory(workingDirectory.path());
		proc.startDetached  (executable, args );
	}

	return true;
}
