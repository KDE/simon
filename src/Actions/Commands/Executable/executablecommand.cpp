#include "executablecommand.h"
#include <QIcon>
#include <QObject>
#include <QProcess>
#include <QVariant>
#include "../SimonLib/WindowsLib/registrymanager.h"


const QString ExecutableCommand::staticCategoryText()
{
	return QObject::tr("Programm");
}

const QString ExecutableCommand::getCategoryText() const
{
	return ExecutableCommand::staticCategoryText();
}

const QIcon ExecutableCommand::staticCategoryIcon()
{
	return QIcon(":/images/icons/applications-system.svg");
}

const QIcon ExecutableCommand::getCategoryIcon() const
{
	return ExecutableCommand::staticCategoryIcon();
}

const QMap<QString,QVariant> ExecutableCommand::getValueMapPrivate() const
{
	QMap<QString,QVariant> out;
	out.insert(tr("Ausführbare Datei"), getExecutable());
	out.insert(tr("Arbeitspfad"), getWorkingDirectory());
	return out;
}

bool ExecutableCommand::triggerPrivate()
{
#ifdef linux
	QString executable;
	QStringList args;
	if (exe.contains(" "))
	{
		executable = exe.left(exe.indexOf(" "));
		QString argumentStrs = exe.mid(executable.count());
		args = argumentStrs.split(" ", QString::SkipEmptyParts);
	} else executable = exe;

	QProcess::startDetached  (executable, args, workingDirectory.toString());
#endif

	#ifdef __WIN32
	RegistryManager *rm = new RegistryManager();
	rm->startProcess(exe, workingDirectory.toString());
	delete rm;
	#endif

	return true;
}
