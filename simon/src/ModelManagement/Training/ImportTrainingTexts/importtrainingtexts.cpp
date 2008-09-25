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


#include "importtrainingtexts.h"
#include <QWizardPage>
#include <QLabel>
#include <KListWidget>
#include <KLineEdit>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QProgressBar>
#include <QRadioButton>
#include <KPushButton>
#include <QFileDialog>
#include "../../../SimonLib/QuickDownloader/quickdownloader.h"
#include "xmltrainingtextlist.h"
#include "importtrainingtextlocalpage.h"
#include "importtrainingtextremotepage.h"
#include "importtrainingtextworkingpage.h"
#include "importtrainingtextselectsourcepage.h"
#include "../xmltrainingtext.h"
#include <KStandardDirs>


/**
 * \brief Constructor
 * \author Peter Grasch
 */
ImportTrainingTexts::ImportTrainingTexts(QWidget *parent) : QWizard(parent)
{
	this->addPage(createIntroPage());
	
	QWizardPage *source = createSourcePage();
	QWizardPage *local = createLocalImportPage();
	QWizardPage *remote = createRemoteImportPage();
	QWizardPage *working = createWorkingPage();
	
	this->addPage(source);
	this->addPage(local);
	this->addPage(remote);
	this->addPage(working);
	
	this->addPage(createFinishedPage());
	setWindowTitle(i18n("Trainingstext importieren"));
	setPixmap(QWizard::WatermarkPixmap, QPixmap(KStandardDirs::locate("appdata", "themes/default/importtexts.png")));
	prevId = 0;
	
}

/**
 * \brief Shows the wizard
 * \author Peter Grasch
 */
void ImportTrainingTexts::start()
{
	show();
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
ImportTrainingTexts::~ImportTrainingTexts()
{
    delete fd;
}


/**
 * \brief Creates the intropage
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createIntroPage()
{
	QWizardPage *intro = new QWizardPage(this);
	intro->setTitle(i18n("Importieren eines neuen Trainingstextes"));
	QLabel *label = new QLabel(intro);
	label->setWordWrap(true);
	label->setText(i18n("Mit Hilfe dieses Assistenten können Sie neue Trainingstexte aus dem Internet oder aus lokalen Dateien importieren.\n\nSo wird das Trainieren von simon nie langweilig!"));
	QVBoxLayout *layout = new QVBoxLayout(intro);
	layout->addWidget(label);
	intro->setLayout(layout);
	
	return intro;
}

/**
 * \brief Creates the remoteimport
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createRemoteImportPage()
{
	ImportTrainingTextRemotePage *remoteImport = new ImportTrainingTextRemotePage(this);
	
	
	return remoteImport;
}

/**
 * \brief Creates the localimportpage
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createLocalImportPage()
{
	ImportTrainingTextLocalPage *localImport = new ImportTrainingTextLocalPage(this);
	
	return localImport;
}


/**
 * \brief Creates the sourcepage
 * 
 * Here you can select where you want to import from (internet/file)
 * 
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createSourcePage()
{
	ImportTrainingTextSelectSourcePage *source = new ImportTrainingTextSelectSourcePage(this);
	return source;
}

/**
 * \brief Creates the working page
 * \author Peter Grasch
 * @return the wizardpage
 */
QWizardPage* ImportTrainingTexts::createWorkingPage()
{
	ImportTrainingTextWorkingPage *working= new ImportTrainingTextWorkingPage(this);
	return working;
}

/**
 * \brief Creates the finished-page
 * @return the QWizardPage
 */
QWizardPage* ImportTrainingTexts::createFinishedPage()
{
	QWizardPage *finished = new QWizardPage(this);
	finished->setTitle(i18n("Text hinzugefügt"));
	QLabel *label = new QLabel(finished);
	label->setWordWrap(true);
	label->setText(i18n("Hiermit haben Sie den neuen Text hinzugefügt.\n\nEr wird nun neben allen anderen Texten in Ihrem Trainingsdialog gelistet.\n\nVielen Dank und viel Spaß mit dem neuen Text!"));
	QVBoxLayout *layout = new QVBoxLayout(finished);
	layout->addWidget(label);
	finished->setLayout(layout);
	
	return finished;
}

