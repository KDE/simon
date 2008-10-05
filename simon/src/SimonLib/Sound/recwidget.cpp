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

#include "wavrecorder.h"
#include "wavplayer.h"
#include "../PostProcessing/postprocessing.h"
#include "logging/logger.h"
#include "coreconfiguration.h"

#include <QGroupBox>
#include <QLabel>
#include <KPushButton>
#include <QSlider>
#include <QString>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QIcon>
#include <KMessageBox>
#include <QFile>
#include <QChar>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param QString name
 * The name that is displayed in the title of the groupbox
 * @param QString filename
 * The filename to record to; 
 * We will ressamble the file (existing or not) when we create the play/pause/delete handles
 * @param QWidget *parent
 * The parent of the object
 */
RecWidget::RecWidget(QString name, QString filename, QWidget *parent) : QWidget(parent)
{	
	this->filename = filename;
	
	rec = new WavRecorder(this);
	play = new WavPlayer(this);

	postProc = new PostProcessing();
	
	ui.setupUi(this);

	ui.gb->setTitle(name);
	
	if (QFile::exists(filename))
	{
		ui.pbRecord->setEnabled(false);
		ui.pbPlay->setEnabled(true);
		ui.pbDelete->setEnabled(true);
	} else 
	{
		ui.pbRecord->setEnabled(true);
		ui.pbPlay->setEnabled(false);
		ui.pbDelete->setEnabled(false);
	}
	
	setupSignalsSlots();

	ui.pbRecord->setIcon(KIcon("media-record"));
	ui.pbPlay->setIcon(KIcon("media-playback-start"));
	ui.pbDelete->setIcon(KIcon("edit-delete"));
}


/**
 * \brief Returns true if there is a file at the assigned filename
 * \author Peter Grasch
 * @return File exists?
 */
bool RecWidget::hasRecordingReady()
{
	return QFile::exists(this->filename);
}

/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void RecWidget::setupSignalsSlots()
{
	//Enable / Disable
	connect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbPlay, SLOT(setDisabled(bool)));
	connect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbDelete, SLOT(setDisabled(bool)));
	connect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbRecord, SLOT(setEnabled(bool)));
	connect(ui.pbPlay, SIGNAL(toggled(bool)), ui.pbDelete, SLOT(setDisabled(bool)));
	connect(ui.pbDelete, SIGNAL(clicked(bool)), ui.pbRecord, SLOT(setDisabled(bool)));
	connect(ui.pbDelete, SIGNAL(clicked(bool)), ui.pbPlay, SLOT(setEnabled(bool)));
	connect(ui.pbDelete, SIGNAL(clicked(bool)), ui.pbDelete, SLOT(setEnabled(bool)));
	
	connect(ui.pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	connect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
	connect(ui.pbDelete, SIGNAL(clicked()), this, SLOT(deleteSample()));
	
	connect(rec, SIGNAL(currentProgress(int)), this, SIGNAL(progress(int)));
	connect(play, SIGNAL(currentProgress(int)), this, SIGNAL(progress(int)));
	connect(rec, SIGNAL(currentProgress(int)), this, SLOT(displayRecordingProgress(int)));
	connect(rec, SIGNAL(currentProgress(int)), this, SLOT(displayPosition(int)));
	connect(play, SIGNAL(currentProgress(int)), this, SLOT(displayPlaybackProgress(int)));
	connect(play, SIGNAL(currentProgress(int)), this, SLOT(displayPosition(int)));
	
	connect(play, SIGNAL(finished()), this, SLOT(finishPlayback()));
}


/**
 * \brief Displays the current position
 * 
 * Moves the slider to the given msecs
 *
 * @param int msecs
 * Timecode to move to
 */
void RecWidget::displayPosition(int msecs)
{
	ui.pbProgress->setValue(msecs);
}


/**
 * \brief Sets the widgets title to the given title
 * \author Peter Grasch
 * @param newTitle The new title
 */
void RecWidget::setTitle(QString newTitle)
{
	ui.gb->setTitle(newTitle);
}


/**
 * \brief Displays the given progress in recording
 * \author Peter Grasch
 * Sets the Label to a string similar to "00:00 / xx:xx"
 * @param int msecs
 * How long have we been recording?
 */
void RecWidget::displayRecordingProgress(int msecs)
{
 	QString textprog = QString("%1").arg((int) msecs/10, 4, 10, QChar('0'));
	textprog.insert(textprog.length()-2, ':');
// 	ui.lbProgress->setText("00:00 / "+textprog);
// 	ui.hsProgress->setMaximum(msecs);
}

/**
 * \brief Displays the given progress in playing
 * \author Peter Grasch
 * @param int msecs
 * How long have we been playing?
 */
void RecWidget::displayPlaybackProgress(int msecs)
{
 	QString textprog = QString("%1").arg(QString::number(msecs/10), 4, QChar('0'));
	textprog.insert(2, ':');
	
	ui.pbProgress->setFormat(ui.pbProgress->format().replace(0,5, textprog));
}


/**
 * \brief Starts the recording
 * \author Peter Grasch
 */
void RecWidget::record()
{
	QString fName = this->filename;
	if (CoreConfiguration::processInternal())
		fName += "_tmp";
	if (!rec->record(fName))
	{
		disconnect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbPlay, SLOT(setDisabled(bool)));
		disconnect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbDelete, SLOT(setDisabled(bool)));
		disconnect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbRecord, SLOT(setEnabled(bool)));
		
		ui.pbRecord->toggle();
		
		connect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbPlay, SLOT(setDisabled(bool)));
		connect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbDelete, SLOT(setDisabled(bool)));
		connect(ui.pbRecord, SIGNAL(toggled(bool)), ui.pbRecord, SLOT(setEnabled(bool)));
	}else {
		disconnect(ui.pbRecord, SIGNAL(clicked()), this, SLOT(record()));
		connect(ui.pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
	}
	emit recording();
}


/**
 * \brief Finishes up the playback
 * \author Peter Grasch
 */
void RecWidget::finishPlayback()
{
	
	disconnect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	ui.pbPlay->setChecked(false);
	connect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
	emit playbackFinished();
}

/**
 * \brief Stops the recording
 * \author Peter Grasch
 */
void RecWidget::stopRecording()
{
	QString fName = this->filename;
	if (CoreConfiguration::processInternal())
		fName += "_tmp";

	if (!rec->finish())
		KMessageBox::error(this, i18n("Abschließen der Aufnahme fehlgeschlagen. Möglicherweise ist die Aufnahme fehlerhaft.\n\nTip: überprüfen Sie ob Sie die nötigen Berechtigungen besitzen um auf %1 schreiben zu dürfen!", fName));
		
	if (CoreConfiguration::processInternal())
// 		if (!QFile::copy(fName, filename) || !QFile::remove(fName))
		if (!postProc->process(fName, filename, true))
			KMessageBox::error(this, i18n("Nachbearbeitung fehlgeschlagen"));
	
	
	ui.pbProgress->setValue(0);
	disconnect(ui.pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
	connect(ui.pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	emit recordingFinished();
}

/**
 * \brief Stops the playback
 * \author Peter Grasch
 */
void RecWidget::stopPlayback()
{
	play->stop();
}

/**
 * \brief Starts the playback
 * \author Peter Grasch
 */
void RecWidget::playback()
{
	if (play->play(this->filename))
	{
		disconnect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
		connect(ui.pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
		emit playing();
	} else ui.pbPlay->setChecked(false);
}

/**
 * \brief Deletes the file at filename (member)
 * \author Peter Grasch
 */
void RecWidget::deleteSample()
{
	if(QFile::remove(this->filename))
	{
		ui.pbProgress->setValue(0);
		ui.pbProgress->setFormat("00:00 / 00:00");
		ui.pbDelete->setEnabled(false);
		ui.pbRecord->setEnabled(true);
		ui.pbPlay->setEnabled(false);
		emit sampleDeleted();
	} else KMessageBox::error(this, 
			i18n("Konnte die Datei %1 nicht entfernen", this->filename));
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
RecWidget::~RecWidget()
{
	delete play;
 	delete rec;
}


