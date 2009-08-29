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
#include <QDomDocument>
#include <QVariant>
#include <KIcon>
#include <KProcess>
#include <KLocalizedString>

ExecutableCommand::ExecutableCommand() : Command("", "")
{
}

ExecutableCommand* ExecutableCommand::createCommand(const QDomElement& elem)
{
	ExecutableCommand *e = new ExecutableCommand();
	if (!e->deSerialize(elem)) {
		delete e;
		e = NULL;
	}
	return e;
}

const QString ExecutableCommand::staticCategoryText()
{
	return i18n("Program");
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
	out.insert(i18n("Executable"), getExecutable());
	out.insert(i18n("Working directory"), QVariant(getWorkingDirectory()));
	return out;
}

bool ExecutableCommand::triggerPrivate()
{
	QStringList commands = exe.split(";");
	KProcess proc;
	for (int i=0; i < commands.count(); i++)
	{
		QString thisExe = commands[i].trimmed();

		QStringList coms = thisExe.split(" ");
		if (coms.isEmpty()) return false;
		QStringList realSplitCommand;
		QString currentItem;
		bool isQuoted=false;
		foreach (QString com, coms)
		{
			if ((com.startsWith("\"")) && (com.endsWith("\"")))
			{
				com.remove(QRegExp("^\""));
				com.remove(QRegExp("\"$"));
				realSplitCommand << com;
			} else
			if ((com.startsWith("\"")) || (com.endsWith("\"")))
			{
				com.remove(QRegExp("^\""));
				com.remove(QRegExp("\"$"));

				currentItem+= " "+com;

				if (isQuoted)
				{
					realSplitCommand << currentItem.trimmed();
					currentItem="";
					isQuoted=false;
				} else isQuoted=true; //begin quote
			} else {
				if (isQuoted) currentItem += " "+com;
				else realSplitCommand << com;
			}
		}
		
		if (realSplitCommand.isEmpty()) continue;
		QString realExecutable = realSplitCommand.takeAt(0);
		proc.setWorkingDirectory(workingDirectory.path());
		proc.setProgram(realExecutable, realSplitCommand);
		proc.startDetached();
	}
	return true;
}


bool ExecutableCommand::deSerialize(const QDomElement& elem)
{
	if (elem.isNull()) return false;

	QDomElement name = elem.firstChildElement();
	QDomElement icon = name.nextSiblingElement();
	QDomElement workingDir = icon.nextSiblingElement();
	QDomElement executable = workingDir.nextSiblingElement();
	this->triggerName = name.text();
	this->iconSrc = icon.text();
	this->workingDirectory = workingDir.text();
	this->exe = executable.text();
	return true;
}

QDomElement ExecutableCommand::serialize(QDomDocument *doc)
{
	QDomElement command = doc->createElement("command");
	QDomElement name = doc->createElement("name");
	name.appendChild(doc->createTextNode(getTrigger()));
	QDomElement icon = doc->createElement("icon");
	icon.appendChild(doc->createTextNode(getIconSrc()));
	QDomElement workingDir = doc->createElement("workingdirectory");
	workingDir.appendChild(doc->createTextNode(getWorkingDirectory().url()));
	QDomElement executable = doc->createElement("executable");
	executable.appendChild(doc->createTextNode(getExecutable()));
	
	command.appendChild(name);
	command.appendChild(icon);
	command.appendChild(workingDir);
	command.appendChild(executable);
		
	return command;
}

