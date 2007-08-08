#include "recwidget.h"


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
RecWidget::RecWidget(QString name, QString filename, QWidget *parent) : QGroupBox(parent)
{	
	this->filename = filename;
	this->setTitle(name);
	
	rec = new WavRecorder(this);
	play = new WavPlayer(this);
	
	setupWidgets();
	setupLayout();
	
	if (QFile::exists(filename))
	{
		pbRecord->setEnabled(false);
		pbPlay->setEnabled(true);
		pbDelete->setEnabled(true);
	}
	
	setupSignalsSlots();
	
}

/**
 * \brief Sets up the signal/slot connections
 * \author Peter Grasch
 */
void RecWidget::setupSignalsSlots()
{
	//Enable / Disable
	connect(pbRecord, SIGNAL(toggled(bool)), pbPlay, SLOT(setDisabled(bool)));
	connect(pbRecord, SIGNAL(toggled(bool)), pbDelete, SLOT(setDisabled(bool)));
	connect(pbRecord, SIGNAL(toggled(bool)), pbRecord, SLOT(setEnabled(bool)));
	connect(pbPlay, SIGNAL(toggled(bool)), pbDelete, SLOT(setDisabled(bool)));
	connect(pbDelete, SIGNAL(clicked(bool)), pbRecord, SLOT(setDisabled(bool)));
	connect(pbDelete, SIGNAL(clicked(bool)), pbPlay, SLOT(setEnabled(bool)));
	connect(pbDelete, SIGNAL(clicked(bool)), pbDelete, SLOT(setEnabled(bool)));
	
	connect(pbRecord, SIGNAL(clicked()), this, SLOT(record()));
	connect(pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
	connect(pbDelete, SIGNAL(clicked()), this, SLOT(deleteSample()));
	
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
	hsProgress->setValue(msecs);
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
	lbProgress->setText("00:00 / "+textprog);
	hsProgress->setMaximum(msecs);
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
	
	lbProgress->setText(lbProgress->text().replace(0,5, textprog));
}


/**
 * \brief Starts the recording
 * \author Peter Grasch
 */
void RecWidget::record()
{
	if (!rec->record(this->filename, 2, 44100))
	{
		disconnect(pbRecord, SIGNAL(toggled(bool)), pbPlay, SLOT(setDisabled(bool)));
		disconnect(pbRecord, SIGNAL(toggled(bool)), pbDelete, SLOT(setDisabled(bool)));
		disconnect(pbRecord, SIGNAL(toggled(bool)), pbRecord, SLOT(setEnabled(bool)));
		
		pbRecord->toggle();
		
		connect(pbRecord, SIGNAL(toggled(bool)), pbPlay, SLOT(setDisabled(bool)));
		connect(pbRecord, SIGNAL(toggled(bool)), pbDelete, SLOT(setDisabled(bool)));
		connect(pbRecord, SIGNAL(toggled(bool)), pbRecord, SLOT(setEnabled(bool)));
	}else {
		disconnect(pbRecord, SIGNAL(clicked()), this, SLOT(record()));
		connect(pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
	}
	emit recording();
}

#include "logger.h"

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
}

/**
 * \brief Stops the recording
 * \author Peter Grasch
 */
void RecWidget::stopRecording()
{
	if (!rec->finish()) 
		QMessageBox::critical(this, tr("Aufnehmen fehlgeschlagen"), QString(tr("Abschlieöen der Aufnahme fehlgeschlagen. Möglicherweise ist die Aufnahme fehlerhaft.\n\nTip: überprüfen Sie ob Sie die nötigen Berechtigungen besitzen um auf %1 schreiben zu dürfen!")).arg(this->filename));
	
	disconnect(pbRecord, SIGNAL(clicked()), this, SLOT(stopRecording()));
	connect(pbRecord, SIGNAL(clicked()), this, SLOT(record()));
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
		disconnect(pbPlay, SIGNAL(clicked()), this, SLOT(playback()));
		connect(pbPlay, SIGNAL(clicked()), this, SLOT(stopPlayback()));
		emit playing();
	} else pbPlay->setChecked(false);
}

/**
 * \brief Deletes the file at filename (member)
 * \author Peter Grasch
 */
void RecWidget::deleteSample()
{
	if(QFile::remove(this->filename))
	{
		hsProgress->setValue(0);
		lbProgress->setText("00:00 / 00:00");
		pbDelete->setEnabled(false);
		pbRecord->setEnabled(true);
		pbPlay->setEnabled(false);
		emit sampleDeleted();
	} else QMessageBox::critical(this, tr("Fehler beim Löschen"), 
			QString(tr("Konnte die Datei %1 nicht entfernen")).arg(this->filename));
}


/**
 * \brief Sets up the GUI Widgets
 * \author Peter Grasch
 * \see setupLayout() setupSignalsSlots()
 */
void RecWidget::setupWidgets()
{
	pbRecord = new QPushButton(QIcon(":/images/icons/media-record.svg"), tr("Aufnehmen"), 
				   this);
	pbPlay = new QPushButton(QIcon(":/images/icons/media-playback-start.svg"), tr("Abspielen"),
				 this);
	pbDelete = new QPushButton(QIcon( ":/images/icons/emblem-unreadable.svg"), tr("Löschen"),
				   this);
	
	hsProgress = new QSlider(Qt::Horizontal, this);
	hsProgress->setValue(0);
	hsProgress->setMaximum(0);
	lbProgress = new QLabel("00:00 / 00:00", this);
	
	pbRecord->setCheckable(true);
	pbPlay->setCheckable(true);
	pbPlay->setEnabled(false);
	pbDelete->setEnabled(false);
}

/**
 * \brief Sets up the Layouts of the GUI Widgets
 * \author Peter Grasch
 * \see setupWidgets() setupSignalsSlots()
 */
void RecWidget::setupLayout()
{
	QVBoxLayout *thisLayout = new QVBoxLayout(this);
	QHBoxLayout *lay1 = new QHBoxLayout();
	QHBoxLayout *lay2 = new QHBoxLayout();
	
	lay1->addWidget(pbRecord);
	lay1->addWidget(pbPlay);
	lay1->addWidget(pbDelete);
	
	lay2->addWidget(hsProgress);
	lay2->addWidget(lbProgress);
	
	thisLayout->insertLayout(0, lay1);
	thisLayout->insertLayout(1, lay2);
}


/**
 * \brief Destructor
 * \author Peter Grasch
 */
RecWidget::~RecWidget()
{
	delete pbRecord;
	delete pbPlay;
	delete pbDelete;
	delete hsProgress;
	delete lbProgress;
	delete play;
 	delete rec;
}


