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

#include "wavfilewidget.h"

#include "wavrecorderclient.h"
#include "wavplayerclient.h"
#include "postprocessing.h"
#include "soundconfig.h"

#include "ui_wavfilewidget.h"

#include <KPushButton>
#include <QProgressBar>
#include <QString>
#include <QFile>
#include <QFont>

#include <KIcon>
#include <KMessageBox>
#include <KLocalizedString>

#include <QPlainTextEdit>


/**
 * \brief Constructor
 * \author Peter Grasch
 * @param QString name
 * The name that is displayed in the title of the groupbox
 * @param QString m_filename
 * The filename to record to; 
 * We will ressamble the file (existing or not) when we create the play/pause/delete handles
 * @param QWidget *parent
 * The parent of the object
 */
WavFileWidget::WavFileWidget(const QString& device, int channels, int sampleRate, 
		const QString& filename, QWidget *parent) : QWidget(parent),
	ui(new Ui::WavFileWidgetUi()), m_device(device), m_filename(filename), postProc(NULL)
{	

	recordingProgress=0;

	isRecording = false;
	isPlaying = false;
	
	rec = new WavRecorderClient(SimonSound::DeviceConfiguration(device, channels, sampleRate), this);
	play = new WavPlayerClient(this);

	ui->setupUi(this);
	ui->pbPlay->setIcon(KIcon("media-playback-start"));
	ui->pbDelete->setIcon(KIcon("edit-delete"));

	connect(ui->pbMoreInformation, SIGNAL(clicked()), this, SLOT(displayClippingWarning()));
	ui->wgWarning->hide();

	if (QFile::exists(m_filename))
	{
		ui->pbPlay->setEnabled(true);
		ui->pbDelete->setEnabled(true);
	} else 
	{
		ui->pbPlay->setEnabled(false);
		ui->pbDelete->setEnabled(false);
	}
	
	setupSignalsSlots();
}

/**
 * \brief Returns true if there is a file at the assigned filename
 * \author Peter Grasch
 * @return File exists?
 */
bool WavFileWidget::hasRecordingReady()
{
	return QFile::exists(m_filename);
}

/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void WavFileWidget::setupSignalsSlots()
{
	connect(ui->pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
	connect(ui->pbDelete, SIGNAL(clicked()), this, SLOT(deleteSample()));
	
	connect(rec, SIGNAL(currentProgress(int, float)), this, SIGNAL(progress(int)));
	connect(play, SIGNAL(currentProgress(int)), this, SIGNAL(progress(int)));
	connect(rec, SIGNAL(currentProgress(int, float)), this, SLOT(displayRecordingProgress(int, float)));
	connect(play, SIGNAL(currentProgress(int)), this, SLOT(displayPlaybackProgress(int)));
	connect(rec, SIGNAL(clippingOccured()), ui->wgWarning, SLOT(show()));
	
	connect(play, SIGNAL(finished()), this, SLOT(finishPlayback()));
}


void WavFileWidget::displayClippingWarning()
{
	KMessageBox::information(this, i18n("simon detected that your volume is set too high. Because of this, clipping has occurred.\n\nPlease lower the volume and re-record this sample."));
}


/**
 * \brief Displays the given progress in recording
 * \author Peter Grasch
 * Sets the Label to a string similar to "00:00 / xx:xx"
 * @param int msecs
 * How long have we been recording?
 */
void WavFileWidget::displayRecordingProgress(int msecs, float level)
{
 	QString textprog = QString("%1").arg((int) msecs/10, 4, 10, QChar('0'));
	textprog.insert(textprog.length()-2, ':');
	ui->pbProgress->setFormat("00:00 / "+textprog);
	this->recordingProgress = msecs;
	ui->pbProgress->setValue(100*level);
}

/**
 * \brief Displays the given progress in playing
 * \author Peter Grasch
 * @param int msecs
 * How long have we been playing?
 */
void WavFileWidget::displayPlaybackProgress(int msecs)
{
 	QString textprog = QString("%1").arg(QString::number(msecs/10), 4, QChar('0'));
	textprog.insert(2, ':');
	
	ui->pbProgress->setFormat(ui->pbProgress->format().replace(0,5, textprog));
	ui->pbProgress->setValue(msecs);
}


/**
 * \brief Starts the recording
 * \author Peter Grasch
 */
void WavFileWidget::record()
{
	QString fName = m_filename;
	if (SoundConfiguration::processInternal())
		fName += "_tmp";

	ui->pbPlay->setEnabled(false);
	ui->pbDelete->setEnabled(false);
	ui->wgWarning->hide();

	if (!rec->record(fName))
	{
		KMessageBox::error(this, i18n("Couldn't start recording.\n\n"
						"The input device \"%1\" could not be initialized.\n\n"
						"Please check your sound configuration and try again.", m_device));
	}else {
		ui->pbProgress->setMaximum(100);
		isRecording = true;
	}
	emit recording();
}


/**
 * \brief Finishes up the playback
 * \author Peter Grasch
 */
void WavFileWidget::finishPlayback()
{
	disconnect(ui->pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	ui->pbPlay->setChecked(false);
	connect(ui->pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
	
	ui->pbDelete->setEnabled(true);

	displayPlaybackProgress(recordingProgress);

	isPlaying = false;
	emit playbackFinished();
}

/**
 * \brief Stops the recording
 * \author Peter Grasch
 */
void WavFileWidget::stopRecording()
{
	if (!isRecording) return;

	QString fName = m_filename;
	bool processInternal = SoundConfiguration::processInternal();
	
	if (processInternal)
		fName += "_tmp";

	if (!rec->finish()) {
		KMessageBox::error(this, i18n("Could not finalize the Sample. "
					"The recording probably failed.\n\n"
					"Tip: Check if you have the needed permissions to write to \"%1\"!", fName));
	} else {
		ui->pbPlay->setEnabled(true);
		ui->pbDelete->setEnabled(true);

		if (processInternal) {

			if (!postProc) {
				postProc = new PostProcessing();
				connect(postProc, SIGNAL(error(const QString&)), this, SIGNAL(error(const QString&)));
			}
			if (!postProc->process(fName, m_filename, true))
				KMessageBox::error(this, i18n("Post-Processing failed"));
		}
	}
	
	
	ui->pbProgress->setValue(0);
	ui->pbProgress->setMaximum(1);
	
	emit recordingFinished();

	isRecording = false;
}

/**
 * \brief Stops the playback
 * \author Peter Grasch
 */
void WavFileWidget::stopPlayback()
{
	if (!isPlaying) return;

	play->stop();
	isPlaying = false;
}

/**
 * \brief Starts the playback
 * \author Peter Grasch
 */
void WavFileWidget::playback()
{
	if (play->play(m_filename))
	{	
		ui->pbProgress->setMaximum((recordingProgress) ? recordingProgress : 1);
		disconnect(ui->pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
		connect(ui->pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
		ui->pbDelete->setEnabled(false);
		isPlaying = true;
		emit playing();
	} else {
		KMessageBox::error(this, i18n("Couldn't start playback.\n\n"
						"The output device could not be initialized.\n\n"
						"Please check your sound configuration and try again."));
		ui->pbPlay->setChecked(false);
	}
}

/**
 * \brief Deletes the file at m_filename (member)
 * \author Peter Grasch
 */
bool WavFileWidget::deleteSample()
{
	if(QFile::remove(m_filename))
	{
		ui->pbProgress->setValue(0);
		ui->pbProgress->setFormat("00:00 / 00:00");
		ui->pbPlay->setEnabled(false);
		ui->pbDelete->setEnabled(false);
		emit sampleDeleted();
		ui->wgWarning->hide();
		return true;
	} else {
		if (QFile::exists(m_filename))
		{
			KMessageBox::error(this, 
				i18n("Couldn't remove file %1", m_filename));
			return false;
		}
	}

	return true;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
WavFileWidget::~WavFileWidget()
{
	delete play;
 	delete rec;
	delete postProc;
}



