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

	isRecording = false;
	isPlaying = false;
	
	ui->setupUi(this);
	ui->pbRecord->setIcon(KIcon("media-record"));
	ui->pbDeleteAll->setIcon(KIcon("edit-delete"));
	connect(ui->pbMoreInformation, SIGNAL(clicked()), this, SLOT(displayClippingWarning()));
	ui->wgWarning->hide();

	setTitle(name);
	ui->tePrompt->setPlainText(text);

	/*
	if (QFile::exists(this->filename))
	{
		ui->pbRecord->setEnabled(false);
		ui->pbDeleteAll->setEnabled(true);
	} else 
	{
		ui->pbRecord->setEnabled(true);
		ui->pbDeleteAll->setEnabled(false);
	}
	*/
	
	setupSignalsSlots();
	initialize();
}


void RecWidget::registerDevice(const QString& id, int channels, int sampleRate, const QString& filenameSuffix)
{
	WavFileWidget *wg = new WavFileWidget(id, channels, sampleRate, fileTemplate+filenameSuffix+".wav", this);

	QBoxLayout *lay = dynamic_cast<QVBoxLayout*>(ui->gbContainer->layout());

	Q_ASSERT(lay);

	lay->addWidget(wg);
}

void RecWidget::initialize()
{
	QStringList soundInputDevices = SoundConfiguration::soundInputDevices();
	QList<int> soundInputChannels = SoundConfiguration::soundInputChannels();
	QList<int> soundInputSampleRates = SoundConfiguration::soundInputSampleRates();
	QList<int> soundInputUses = SoundConfiguration::soundInputUses();

	for (int i=0; i < soundInputDevices.count(); i++)
	{
		if (!(soundInputUses[i] & SimonSound::Training))
			continue;

		registerDevice(soundInputDevices[i], soundInputChannels[i], soundInputSampleRates[i], "."+QString::number(i));
	}



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
	//FIXME
//	return QFile::exists(this->filename);
	return false;
}

/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void RecWidget::setupSignalsSlots()
{
	connect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	connect(ui->pbDeleteAll, SIGNAL(clicked()), this, SLOT(deleteSample()));
}


void RecWidget::displayClippingWarning()
{
	KMessageBox::information(this, i18n("simon detected that your volume is set too high. Because of this, clipping has occurred.\n\nPlease lower the volume and re-record this sample."));
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
	//TODO
//	QString fName = this->fileTemplate;
//	if (SoundConfiguration::processInternal())
//		fName += "_tmp";

	ui->pbRecord->setEnabled(true);
	ui->pbDeleteAll->setEnabled(false);
	ui->wgWarning->hide();

	emit recording();
}


/**
 * \brief Stops the recording
 * \author Peter Grasch
 */
void RecWidget::stopRecording()
{
	if (!isRecording) return;

	//TODO
//	QString fName = this->filename;
	//bool processInternal = SoundConfiguration::processInternal();
	
	//if (processInternal)
	//	fName += "_tmp";

	
	ui->pbRecord->setChecked(false);
	
	disconnect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
	connect(ui->pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	emit recordingFinished();

	isRecording = false;
}

void RecWidget::stopPlayback()
{
	//TODO
	
}

/**
 * \brief Deletes the file at fileTemplate (member)
 * \author Peter Grasch
 */
bool RecWidget::deleteSample()
{
/*	if(QFile::remove(this->fileTemplate))
	{
		ui->pbRecord->setEnabled(true);
		ui->pbDeleteAll->setEnabled(false);
		emit sampleDeleted();
		ui->wgWarning->hide();
		return true;
	} else {
		if (QFile::exists(this->fileTemplate))
		{
			KMessageBox::error(this, 
				i18n("Couldn't remove file %1", this->fileTemplate));
			return false;
		}
	}
	*/

	return true;
}


