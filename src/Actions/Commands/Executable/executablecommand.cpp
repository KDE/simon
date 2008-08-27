#include "executablecommand.h"
#include <QObject>
#include <QVariant>
#include <KIcon>
#include <KProcess>
#include <KLocalizedString>

#ifdef __WIN32
#include "../SimonLib/WindowsLib/registrymanager.h"
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
#ifdef linux
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

	#ifdef __WIN32
	RegistryManager *rm = new RegistryManager();
	rm->startProcess(exe, workingDirectory.toString());
	delete rm;
	#endif

	return true;
}
