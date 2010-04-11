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
#include "postprocessing.h"
#include "soundconfig.h"

#include <simonlogging/logger.h>

#include <QGroupBox>
#include <QLabel>
#include <KPushButton>
#include <QProgressBar>
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
RecWidget::RecWidget(QString name, QString text, QString filename, QWidget *parent) : QWidget(parent), postProc(NULL)
{	
	this->filename = filename;
	recordingProgress=0;

	isRecording = false;
	isPlaying = false;
	
	rec = new WavRecorderClient(this);
	play = new WavPlayerClient(this);

	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////            GUI                    /////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////

	QVBoxLayout *mainLay = new QVBoxLayout(this);
	gbContainer = new QGroupBox(name, this);
	mainLay->addWidget(gbContainer);

	QVBoxLayout *lay = new QVBoxLayout(gbContainer);

	tePrompt = new QPlainTextEdit(this);
	tePrompt->setFont(SoundConfiguration::promptFont());
	tePrompt->setPlainText(text);
	tePrompt->setReadOnly(true);
	lay->addWidget(tePrompt);

	QHBoxLayout *hBox = new QHBoxLayout();
	pbRecord = new KPushButton(KIcon("media-record"),
				    i18n("Record"),
				    this);
	pbRecord->setCheckable(true);
	hBox->addWidget(pbRecord);

	pbPlay = new KPushButton(KIcon("media-playback-start"),
				    i18n("Play"),
				    this);
	pbPlay->setCheckable(true);
	hBox->addWidget(pbPlay);

	pbDelete = new KPushButton(KIcon("edit-delete"),
				    i18n("Delete"),
				    this);
	hBox->addWidget(pbDelete);
	lay->addLayout(hBox);
	
	pbProgress = new QProgressBar(this);
	pbProgress->setMinimum(0);
	pbProgress->setValue(0);
	pbProgress->setMaximum(1);
	pbProgress->setFormat("00:00 / 00:00");
	
	lay->addWidget(pbProgress);


	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////
	
	
	if (QFile::exists(this->filename))
	{
		pbRecord->setEnabled(false);
		pbPlay->setEnabled(true);
		pbDelete->setEnabled(true);
	} else 
	{
		pbRecord->setEnabled(true);
		pbPlay->setEnabled(false);
		pbDelete->setEnabled(false);
	}
	
	setupSignalsSlots();

	resizePromptLabel();
}

void RecWidget::displayError(const QString& error)
{
	KMessageBox::error(this, error);
}

void RecWidget::changePromptFont(const QFont& font)
{
	QString text = tePrompt->toPlainText();
	tePrompt->setFont(font);
	tePrompt->setPlainText(text);
	resizePromptLabel();
}


void RecWidget::resizePromptLabel()
{
//	QTextDocument *doc = tePrompt->document();
//	kDebug() << doc->pageSize() << doc->size() << doc->idealWidth() << doc->textWidth();
	//          QSizeF(331, -1)    QSizeF(331, 688)       315             331
	return;
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
	connect(pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	connect(pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
	connect(pbDelete, SIGNAL(clicked()), this, SLOT(deleteSample()));
	
	connect(rec, SIGNAL(currentProgress(int, float)), this, SIGNAL(progress(int)));
	connect(play, SIGNAL(currentProgress(int)), this, SIGNAL(progress(int)));
	connect(rec, SIGNAL(currentProgress(int, float)), this, SLOT(displayRecordingProgress(int, float)));
	connect(play, SIGNAL(currentProgress(int)), this, SLOT(displayPlaybackProgress(int)));
	
	connect(play, SIGNAL(finished()), this, SLOT(finishPlayback()));
}


/**
 * \brief Sets the widgets title to the given title
 * \author Peter Grasch
 * @param newTitle The new title
 */
void RecWidget::setTitle(QString newTitle)
{
	gbContainer->setTitle(newTitle);
}


/**
 * \brief Displays the given progress in recording
 * \author Peter Grasch
 * Sets the Label to a string similar to "00:00 / xx:xx"
 * @param int msecs
 * How long have we been recording?
 */
void RecWidget::displayRecordingProgress(int msecs, float level)
{
 	QString textprog = QString("%1").arg((int) msecs/10, 4, 10, QChar('0'));
	textprog.insert(textprog.length()-2, ':');
	pbProgress->setFormat("00:00 / "+textprog);
	this->recordingProgress = msecs;
	pbProgress->setValue(100*level);
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
	
	pbProgress->setFormat(pbProgress->format().replace(0,5, textprog));
	pbProgress->setValue(msecs);
}


/**
 * \brief Starts the recording
 * \author Peter Grasch
 */
void RecWidget::record()
{
	QString fName = this->filename;
	if (SoundConfiguration::processInternal())
		fName += "_tmp";

	pbRecord->setEnabled(true);
	pbPlay->setEnabled(false);
	pbDelete->setEnabled(false);

	if (!rec->record(fName))
	{
		KMessageBox::error(this, i18n("Couldn't start recording.\n\n"
						"The input device could not be initialized.\n\n"
						"Please check your sound configuration and try again."));
		pbRecord->setChecked(false);
	}else {
		disconnect(pbRecord, SIGNAL(clicked()), this, SLOT(record()));
		connect(pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
		
		pbProgress->setMaximum(100);
		isRecording = true;
		
		pbRecord->setChecked(true);
	}
	emit recording();
}


/**
 * \brief Finishes up the playback
 * \author Peter Grasch
 */
void RecWidget::finishPlayback()
{
	disconnect(pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
	pbPlay->setChecked(false);
	connect(pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
	emit playbackFinished();
	
	pbDelete->setEnabled(true);

	displayPlaybackProgress(recordingProgress);

	isPlaying = false;
}

/**
 * \brief Stops the recording
 * \author Peter Grasch
 */
void RecWidget::stopRecording()
{
	if (!isRecording) return;

	QString fName = this->filename;
	bool processInternal = SoundConfiguration::processInternal();
	
	if (processInternal)
		fName += "_tmp";

	if (!rec->finish()) {
		KMessageBox::error(this, i18n("Could not finalize the Sample. "
					"The recording probably failed.\n\n"
					"Tip: Check if you have the needed permissions to write to \"%1\"!", fName));
	} else {
		pbRecord->setEnabled(false);
		pbPlay->setEnabled(true);
		pbDelete->setEnabled(true);

		if (processInternal) {

			if (!postProc) {
				postProc = new PostProcessing();
				connect(postProc, SIGNAL(error(const QString&)), this, SLOT(displayError(const QString&)));
			}
			if (!postProc->process(fName, filename, true))
				KMessageBox::error(this, i18n("Post-Processing failed"));
		}
	}
	
	
	pbProgress->setValue(0);
	pbProgress->setMaximum(1);
	pbRecord->setChecked(false);
	
	disconnect(pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
	connect(pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	emit recordingFinished();

	isRecording = false;
}

/**
 * \brief Stops the playback
 * \author Peter Grasch
 */
void RecWidget::stopPlayback()
{
	if (!isPlaying) return;

	play->stop();
	isPlaying = false;
}

/**
 * \brief Starts the playback
 * \author Peter Grasch
 */
void RecWidget::playback()
{
	if (play->play(this->filename))
	{	
		pbProgress->setMaximum((recordingProgress) ? recordingProgress : 1);
		disconnect(pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
		connect(pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
		emit playing();
		pbDelete->setEnabled(false);
		isPlaying = true;
	} else {
		KMessageBox::error(this, i18n("Couldn't start playback.\n\n"
						"The output device could not be initialized.\n\n"
						"Please check your sound configuration and try again."));
		pbPlay->setChecked(false);
	}
}

/**
 * \brief Deletes the file at filename (member)
 * \author Peter Grasch
 */
bool RecWidget::deleteSample()
{
	if(QFile::remove(this->filename))
	{
		pbProgress->setValue(0);
		pbProgress->setFormat("00:00 / 00:00");
		pbRecord->setEnabled(true);
		pbPlay->setEnabled(false);
		pbDelete->setEnabled(false);
		emit sampleDeleted();
		return true;
	} else {
		if (QFile::exists(this->filename))
		{
			KMessageBox::error(this, 
				i18n("Couldn't remove file %1", this->filename));
			return false;
		}
	}

	return true;
}

/**
 * \brief Destructor
 * \author Peter Grasch
 */
RecWidget::~RecWidget()
{
	delete play;
 	delete rec;
	delete postProc;
}


