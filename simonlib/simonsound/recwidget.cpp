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

#include "recwidget.h"

#include "wavrecorderclient.h"
#include "wavplayerclient.h"
#include "soundconfig.h"
#include "simonsound.h"
#include "wavfilewidget.h"

#include <QGroupBox>
#include <QLabel>
#include <KPushButton>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <KLocalizedString>

#include <KIcon>

#include <KMessageBox>
#include <QFile>
#include <QChar>

#include <QFont>
#include <KLocale>
#include <QPlainTextEdit>

#include "ui_recwidget.h"

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param QString name
 * The name that is displayed in the title of the groupbox
 * @param QString fileTemplate
 * The fileTemplate to record to; 
 * We will ressamble the file (existing or not) when we create the play/pause/delete handles
 * @param QWidget *parent
 * The parent of the object
 */
RecWidget::RecWidget(QString name, QString text, QString fileTemplate, QWidget *parent) : QWidget(parent),
	ui(new Ui::RecWidgetUi())
{	
	this->fileTemplate = fileTemplate;

	ui->setupUi(this);
	ui->pbRecord->setIcon(KIcon("media-record"));
	ui->pbDeleteAll->setIcon(KIcon("edit-delete"));

	setTitle(name);
	ui->tePrompt->setPlainText(text);

	setupSignalsSlots();
	initialize();
	connect(SoundServer::getInstance(), SIGNAL(devicesChanged()), this, SLOT(initialize()));
}


void RecWidget::registerDevice(const QString& id, int channels, int sampleRate, const QString& filenameSuffix)
{
	WavFileWidget *wg = new WavFileWidget(id, channels, sampleRate, fileTemplate+filenameSuffix+".wav", this);

	QBoxLayout *lay = dynamic_cast<QVBoxLayout*>(ui->gbContainer->layout());

	Q_ASSERT(lay);

	lay->addWidget(wg);

	connect(wg, SIGNAL(sampleDeleted()), this, SLOT(slotSampleDeleted()));
	connect(wg, SIGNAL(playing()), this, SLOT(slotEnableDeleteAll()));
	connect(wg, SIGNAL(playbackFinished()), this, SLOT(slotEnableDeleteAll()));

	waves << wg;
}

bool RecWidget::isRecording()
{
	foreach (WavFileWidget *wav, waves)
		if (wav->getIsRecording())
			return true;
	return false;
}

QStringList RecWidget::getFileNames()
{
	QStringList fileNames;
	foreach (WavFileWidget *wav, waves)
		if (wav->hasRecordingReady())
			fileNames << wav->getFileName();

	return fileNames;
}

void RecWidget::initialize()
{
	foreach (WavFileWidget *wg, waves)
		wg->deleteLater();
	waves.clear();

	QList<SimonSound::DeviceConfiguration> devices = SoundServer::getTrainingInputDevices();
	for (int i=0; i < devices.count(); i++)
		registerDevice(devices[i].name(), devices[i].channels(), devices[i].sampleRate(), "."+QString::number(i));

	adjustButtonsToFile();
}

void RecWidget::displayError(const QString& error)
{
	KMessageBox::error(this, error);
}

void RecWidget::changePromptFont(const QFont& font)
{
	QString text = ui->tePrompt->toPlainText();
	ui->tePrompt->setFont(font);
	ui->tePrompt->setPlainText(text);
}


/**
 * \brief Returns true if there is a file at the assigned filename
 * \author Peter Grasch
 * @return File exists?
 */
bool RecWidget::hasRecordingReady()
{
	bool recordingReady = false;
	foreach (WavFileWidget *wav, waves)
		recordingReady |= wav->hasRecordingReady();

	return recordingReady;
}

/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void RecWidget::setupSignalsSlots()
{
	connect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	connect(ui->pbDeleteAll, SIGNAL(clicked()), this, SLOT(deleteAll()));
}


/**
 * \brief Sets the widgets title to the given title
 * \author Peter Grasch
 * @param newTitle The new title
 */
void RecWidget::setTitle(QString newTitle)
{
	ui->gbContainer->setTitle(newTitle);
}



/**
 * \brief Starts the recording
 * \author Peter Grasch
 */
void RecWidget::record()
{
	foreach (WavFileWidget *wav, waves)
		wav->record();
	
	bool someoneIsRecording = false;
	foreach (WavFileWidget *wav, waves)
		someoneIsRecording |= wav->getIsRecording();

	ui->pbRecord->setChecked(someoneIsRecording);
	disconnect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	connect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));

	emit recording();
}


/**
 * \brief Stops the recording
 * \author Peter Grasch
 */
void RecWidget::stopRecording()
{
	foreach (WavFileWidget *wav, waves)
		wav->stopRecording();
	
	ui->pbRecord->setChecked(false);
	adjustButtonsToFile();
	
	disconnect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
	connect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	emit recordingFinished();
}

void RecWidget::stopPlayback()
{
	foreach (WavFileWidget *wav, waves)
		wav->stopPlayback();
}


void RecWidget::adjustButtonsToFile()
{
	bool somethingHasSample = hasRecordingReady();

	ui->pbRecord->setEnabled(!somethingHasSample);
	ui->pbDeleteAll->setEnabled(somethingHasSample);
}

void RecWidget::slotSampleDeleted()
{
	adjustButtonsToFile();

	emit sampleDeleted();
}

void RecWidget::slotEnableDeleteAll()
{
	bool shouldEnableDelete = true;
	foreach (WavFileWidget *wav, waves)
	{
		shouldEnableDelete &= ! wav->getIsPlaying();
		kDebug() << wav->getIsPlaying();
	}
	kDebug() << "Updating enable button: " << shouldEnableDelete;
	ui->pbDeleteAll->setEnabled(shouldEnableDelete);
}

/**
 * \brief Deletes the file at fileTemplate (member)
 * \author Peter Grasch
 */
bool RecWidget::deleteAll()
{
	bool success = true;
	foreach (WavFileWidget *wav, waves)
		success = wav->deleteSample() && success;

	return success;
}


