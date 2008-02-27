//
// C++ Implementation: importtrainingdirectoryworkingpage
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "importtrainingdirectoryworkingpage.h"
#include <QPushButton>
#include <QDir>
#include <QPushButton>
#include <QHBoxLayout>
#include <QProgressBar>
#include <QVBoxLayout>
#include "trainingmanager.h"
#include <QMessageBox>
#include <QFileInfo>
#include <QFile>
#include <QDate>
#include <QTime>
#include <QVariant>
#include <QLabel>
#include <QCoreApplication>
#include <QVariant>
#include "settings.h"
#include "postprocessing.h"

/**
 * \brief Constructor - creates the GUI
 * \author Peter Grasch
 * @param parent 
 * Parent of the widget
 */
ImportTrainingDirectoryWorkingPage::ImportTrainingDirectoryWorkingPage(QWidget *parent) : QWizardPage(parent)
{
	setTitle(tr("Verarbeite Ordner..."));
	completed = false;
	
	QVBoxLayout *lay = new QVBoxLayout(this);
	QLabel *lbMain = new QLabel(this);
	lbMain->setText(tr("Der angegebene Ordner wird verarbeit.\nSie können den Fortschritt mit dem Fortschrittsbalken\nüberprüfen.\n\nBitte haben Sie einen Moment Geduld.\n\n"));
	
	pbMain = new QProgressBar(this);
	pbMain->setMaximum(0);

	lay->addWidget(lbMain);
	lay->addWidget(pbMain);
	setLayout(lay);

	this->pp = new PostProcessing();
}


/**
 * \brief Starts the importing process and calls all the other methods
 * \author Peter Grasch
 */
void ImportTrainingDirectoryWorkingPage::initializePage()
{
	completed = false;
	prog=0;
	QString dir = field("directory").toString();
	
	QString wavDestdir = Settings::getS("Model/PathToSamples")+"/";

	QDir d(wavDestdir);
	if (!d.exists())
		if (!d.mkpath(wavDestdir))
		{
			QMessageBox::critical(this, tr("Fehler"), tr("Konnte Ausgabeordner %1 nicht erstellen").arg(wavDestdir));
		}
	
	QStringList *dataFiles = this->searchDir(dir);
	if (!dataFiles) return error();
	

	pbMain->setMaximum(dataFiles->count());
	pbMain->setValue(0);

	dataFiles = processSounds(*dataFiles, wavDestdir);
	if (!dataFiles) return error();
	
	if (!createPrompts(*dataFiles)) return error();
	

	completed = true;

	emit completeChanged();
}


/**
 * \brief Creates the promptsfile from the given stringlist
 * Using extractSaid()
 * \author Peter Grasch
 * \see extractSaid()
 * @param dataFiles The dataFiles to include in the prompts-file
 * @param destDir The destination file
 * @return success
 */
bool ImportTrainingDirectoryWorkingPage::createPrompts(QStringList dataFiles)
{
	TrainingManager *train = TrainingManager::getInstance();
	

	PromptsTable *prompts = new PromptsTable();
	QFileInfo fileInfo;
	QString fileName, said;

	for (int i=0; i <dataFiles.count(); i++)
	{
		fileInfo.setFile(dataFiles[i]);
		fileName = fileInfo.fileName();

		said = extractSaid(fileName);
		prompts->insert(fileName.left(fileName.lastIndexOf(".")), said);
	}
	train->addSamples(prompts);
	train->savePrompts();
	delete prompts;
	return true;
}


/**
 * \brief Extracts the information that has been said out of the source
 * 
 * Returns the cleaned fielname without the extension and in uppercase
 * 
 * \author Peter Grasch
 * 
 * @param source The string to parse
 * @return cleaned string
 */
QString ImportTrainingDirectoryWorkingPage::extractSaid(QString source)
{
	QString said = source.left(source.lastIndexOf("."));
	said.remove(QRegExp("([0-9]|\\.|\\(|\\)|\\[|\\]|\\-)"));
	said.replace("_", " ");
	return said.trimmed();
}

/**
 * \brief Walks the given dir. recursively and returns all ,wav files
 * @param dir The dir to walk
 * @return All files found
 */
QStringList* ImportTrainingDirectoryWorkingPage::searchDir(QString dir)
{
	QDir *dirHandle = new QDir(dir);
	if ((!dirHandle) || (!dirHandle->isReadable())) return NULL;

	QStringList dirsToCheck;
	QStringList *dataFiles = new QStringList();
	dirsToCheck<<dir;

	QStringList allowedFileTypes;
	allowedFileTypes << "*.wav";
	allowedFileTypes << "*.mp3";
	allowedFileTypes << "*.ogg";
	allowedFileTypes << "*.flac";
	QStringList dirs;
	QStringList files;

	while (!dirsToCheck.isEmpty())
	{
		dirHandle->setPath(dirsToCheck.takeAt(0));
		dirs = dirHandle->entryList(QDir::Dirs);
		for (int i=2; i < dirs.count(); i++)
			dirsToCheck.append(dirHandle->path()+"/"+dirs[i]);

		files = dirHandle->entryList(allowedFileTypes,  QDir::Files);
		for (int i=0; i < files.count(); i++)
			dataFiles->append(dirHandle->path()+"/"+files[i]);
	}
	return dataFiles;
}

/**
 * \brief Process the sound files from given in the list to the destDir
 * 
 * Resamples the audio to 16khz and normalizes it afterwards.
 * 
 * @param dataFiles The given datafiles
 * @param destDir The destination directory
 * @return the datafiles - if not successful it returns NULL
 */
QStringList* ImportTrainingDirectoryWorkingPage::processSounds(QStringList dataFiles, 
		QString destDir)
{
	QString newFileName;
	QFileInfo fInfo;
	QStringList *newFiles = new QStringList();
	
	for (int i=0; i < dataFiles.count(); i++)
	{
		fInfo.setFile(dataFiles[i]);
		QString dateTime = QDate::currentDate().toString ( "yyyy-MM-dd" ) +"_"+QTime::currentTime().toString("hh-mm-ss");
		newFileName = destDir+"/"+fInfo.fileName().left(fInfo.fileName().lastIndexOf(".")).replace(" ", "_")+"_"+dateTime+"_.wav";


		if (!pp->process(dataFiles[i], newFileName))
		{
			QMessageBox::critical(this, tr("Fehler"), tr("Konnte Tondaten nicht verarbeiten"));
			return NULL;
		}
		newFiles->append(newFileName);
		pbMain->setValue(++prog);
		QCoreApplication::processEvents();
	}

	
	return newFiles;
}

ImportTrainingDirectoryWorkingPage::~ImportTrainingDirectoryWorkingPage()
{
	delete pp;
    pbMain->deleteLater();
}
