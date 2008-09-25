#include "executablecommand.h"
#include <QObject>
#include <QVariant>
#include <KIcon>
#include <KProcess>
#include <KLocalizedString>
#include <KDebug>

#ifdef Q_OS_WIN
#include "../../../SimonLib/WindowsLib/registrymanager.h"
#endif


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

		#ifdef Q_OS_UNIX
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
		#endif

		#ifdef Q_OS_WIN
		RegistryManager *rm = new RegistryManager();
		rm->startProcess(exe, workingDirectory.path());
		delete rm;
		#endif
	}

	return true;
}
