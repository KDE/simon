/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include <QDomDocument>
#include <QDomElement>
#include <QVariant>
#include <QIcon>
#include <KProcess>
#include <KI18n/klocalizedstring.h>

const QString ExecutableCommand::staticCategoryText()
{
  return i18n("Program");
}


const QString ExecutableCommand::getCategoryText() const
{
  return ExecutableCommand::staticCategoryText();
}


const QIcon ExecutableCommand::staticCategoryIcon()
{
  return QIcon::fromTheme("applications-system");
}


const QIcon ExecutableCommand::getCategoryIcon() const
{
  return ExecutableCommand::staticCategoryIcon();
}


const QMap<QString,QVariant> ExecutableCommand::getValueMapPrivate() const
{
  QMap<QString,QVariant> out;
  out.insert(i18nc("The executable file to launch", "Executable"), getExecutable());
  out.insert(i18n("Working folder"), QVariant(getWorkingDirectory()));
  return out;
}


bool ExecutableCommand::triggerPrivate(int *state)
{
  Q_UNUSED(state);
  QStringList commands = exe.split(';');
  KProcess proc;
  for (int i=0; i < commands.count(); i++) {
    QString thisExe = commands[i].trimmed();

    QStringList coms = thisExe.split(' ');
    if (coms.isEmpty()) return false;
    QStringList realSplitCommand;
    QString currentItem;
    bool isQuoted=false;
    //we need the copy here
    foreach (QString com, coms) { // krazy:exclude=foreach
      if ((com.startsWith('"')) && (com.endsWith('"'))) {
        com.remove(QRegExp(QLatin1String("^\"")));
        com.remove(QRegExp(QLatin1String("\"$")));
        realSplitCommand << com;
      } else
      if ((com.startsWith('"')) || (com.endsWith('"'))) {
        com.remove(QRegExp(QLatin1String("^\"")));
        com.remove(QRegExp(QLatin1String("\"$")));

        currentItem+= ' '+com;

        if (isQuoted) {
          realSplitCommand << currentItem.trimmed();
          currentItem="";
          isQuoted=false;
        } else isQuoted=true;                     //begin quote
      }
      else {
        if (isQuoted) currentItem += ' '+com;
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


QDomElement ExecutableCommand::serializePrivate(QDomDocument *doc, QDomElement& commandElem)
{
  QDomElement workingDir = doc->createElement("workingdirectory");
  workingDir.appendChild(doc->createTextNode(getWorkingDirectory().url()));

  QDomElement executable = doc->createElement("executable");
  executable.appendChild(doc->createTextNode(getExecutable()));

  commandElem.appendChild(workingDir);
  commandElem.appendChild(executable);

  return commandElem;
}


bool ExecutableCommand::deSerializePrivate(const QDomElement& commandElem)
{
  QDomElement exeElem = commandElem.firstChildElement("executable");
  if (exeElem.isNull()) return false;

  QDomElement workingDirectoryElem = commandElem.firstChildElement("workingdirectory");

  exe = exeElem.text();
  workingDirectory = workingDirectoryElem.text();

  return true;
}


STATIC_CREATE_INSTANCE_C(ExecutableCommand);
