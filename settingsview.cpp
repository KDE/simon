//3
// C++ Implementation: settingsview
//
// Description:
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "settingsview.h"
#include "simoninfo.h"
#include "logger.h"
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QCoreApplication>
#include <QMessageBox>
#include "command.h"


/**
 * \brief Constructor
 *
 * This instances a new dialog and connects the signals/slots
 * It also enables/disables the plattform dependant options (alsa/dsound)
 *
 * \author Peter Grasch
 *
 * \param QWidget *parent
 * The parent of the dialog
 */

SettingsView::SettingsView ( QWidget *parent )
		: QDialog ( parent )
{
	ui.setupUi ( this );

	connect ( ui.pbSystemSettings, SIGNAL ( clicked() ), this, SLOT ( switchToSystem() ) );
	connect ( ui.pbSoundSettings, SIGNAL ( clicked() ), this, SLOT ( switchToSound() ) );
	connect ( ui.pbCommandSettings, SIGNAL ( clicked() ), this, SLOT ( switchToCommands() ) );
	connect ( ui.pbProtocolSettings, SIGNAL ( clicked () ), this, SLOT ( switchToProtocols() ) );
	connect ( ui.pbRevert, SIGNAL ( clicked() ), this, SLOT ( switchToHistory() ) );
	connect ( ui.pbApply, SIGNAL ( clicked() ), this, SLOT ( apply() ) );
	connect ( ui.pbConfirm, SIGNAL ( clicked() ), this, SLOT ( apply() ) );
	connect ( ui.cbInDevice, SIGNAL ( currentIndexChanged ( int ) ), this, SLOT ( refreshDeviceCapabilities() ) );
	connect ( ui.pbAddAdress, SIGNAL ( clicked() ), this, SLOT ( addAddress() ) );
	connect ( ui.pbDeleteAdress, SIGNAL ( clicked() ), this, SLOT ( deleteAddress() ) );
	connect ( ui.twCommand, SIGNAL (itemChanged(QTableWidgetItem*)), this, SLOT(saveCommands()));
	connect ( ui.pbNewCommand, SIGNAL (clicked()), this, SLOT (newCommand()));
	connect ( ui.pbDeleteCommand, SIGNAL (clicked()), this, SLOT (deleteCommand()));
	connect (ui.twCommand, SIGNAL (cellDoubleClicked (int, int) ), this, SLOT(activateCb()));
	connect (ui.twCommand, SIGNAL (currentCellChanged(int,int,int,int) ), this, SLOT(deactivateCb(int,int,int,int)));
	connect (ui.pbEditCommand, SIGNAL (clicked()), this, SLOT (editCommand()));
	connect (ui.pbReloadCommands, SIGNAL (clicked()), this, SLOT (reloadCommands()));
	connect (ui.cbShowCommand, SIGNAL (currentIndexChanged ( const QString &) ), this, SLOT(showOnlyCommands()));
	connect (ui.pbClearSearchCommand, SIGNAL(clicked()), this, SLOT(clearSearchLineEdit()));
	connect (ui.leSearchCommand, SIGNAL(textChanged(const QString &)), this, SLOT(searchCommandList()));
	
	connect (ui.cbOnlyDay, SIGNAL (stateChanged(int)), this, SLOT (onlyDay()));
	connect ( ui.cwLogDay, SIGNAL (selectionChanged()), this, SLOT (onlyDay()));

	this->manager = new LogManager();
	if (!this->manager->readLog())
	{		
		QMessageBox::critical(this,tr("Fehler beim Auslesen des Logs"),tr("Beim Auslesen der Logdatei ist ein Fehler aufgetreten.\n\nÜberprüfen Sie ob Sie die benötigten Berechtigugnen besitzen."));
		Logger::log(tr("[ERR] Fehler beim öffnen des Logfilse"));
	}
	else
	{
		this->manager->readLog();
	}
	ui.twCommand->resizeColumnToContents(1);
	
	this->settings = new QSettings ( QSettings::IniFormat,QSettings::UserScope,"CyberByte","simon" );
	this->sc= new SoundControl();
	commandLoader = new XMLCommand("conf/commands.xml");
	this->readConfig();
#ifdef linux
	ui.lbDirectX->setVisible ( false );
	ui.lbALSA->setVisible ( true );
#endif
#ifdef __WIN32
	ui.lbDirectX->setVisible ( true );
	ui.lbALSA->setVisible ( false );
#endif
	this->switchToSystem();
}




void SettingsView::deleteAddress()
{
	ui.cbAddress->removeItem ( ui.cbAddress->currentIndex() );
}

void SettingsView::addAddress()
{
	QString host="";
	QString port="";
	bool ok=false;
	host=QInputDialog::getText ( this, tr("Neue Adresse Hinzufügen"),tr("Host:"), QLineEdit::Normal,"localhost", &ok );
	if ( ok && !host.isEmpty() )
	{
		ok=false;
		port=QInputDialog::getText ( this, tr("Neue Adresse Hinzufügen"),tr("Portnummer:"), QLineEdit::Normal, "4444",&ok );
		if ( ok )
		{
			if ( !port.isEmpty() ) ui.cbAddress->addItem ( host+":"+port );
			else ui.cbAddress->addItem ( host+":4444" );
		}
	}
}

void SettingsView::refreshDeviceCapabilities()
{
	ui.cbChannels->clear();
	ui.cbSampleRate->clear();
	if ( sc->getChannel ( ui.cbInDevice->currentText() ) >1 ) ui.cbChannels->addItem ( "Stereo", 2 );
	if ( sc->getChannel ( ui.cbInDevice->currentText() ) >0 ) ui.cbChannels->addItem ( "Mono", 1 );

	QList<int>* samplerates;
	samplerates=sc->getSamplerate ( ui.cbInDevice->currentText() );
	if ( !samplerates ) return;
	qSort ( samplerates->begin(), samplerates->end(),  qGreater<int>() );
	for ( int i=0; i<samplerates->count(); i++ )
	{
		ui.cbSampleRate->addItem ( QString::number ( samplerates->at ( i ) ) );
	}
}

/**
 * \brief Switches to the "system" tab
 *
 * \author Peter Grasch
 */
void SettingsView::readConfig()
{
	Logger::log(tr("[INF] lade Einstellungen"));
	settings->sync();

	ui.cbAskBeforeExit->setCheckState ( ( settings->value ( "askbeforeexit" ).toBool() ) ? Qt::Checked : Qt::Unchecked );
	ui.cbStartSimonOnBoot->setCheckState ( ( settings->value ( "simonautostart" ).toBool() ) ? Qt::Checked : Qt::Unchecked );
	ui.cbStartJuliusdOnBoot->setCheckState ( ( settings->value ( "juliusdautostart" ).toBool() ) ? Qt::Checked : Qt::Unchecked );
	ui.cbStartJuliusAsNeeded->setCheckState ( ( settings->value ( "juliusdrequired" ).toBool() ) ? Qt::Checked : Qt::Unchecked );
	ui.cbSaveAllRecordings->setCheckState ( ( settings->value ( "saveallrecordings" ).toBool() ) ? Qt::Checked : Qt::Unchecked );
	ui.leLexicon->setText ( settings->value ( "paths/lexicon" ).toString() );
	ui.leGrammar->setText ( settings->value ( "paths/grammar" ).toString() );
	ui.leCommands->setText ( settings->value ( "paths/commando" ).toString() );
	ui.leVocab->setText ( settings->value ( "paths/vocabul" ).toString() );
	ui.lePrompts->setText ( settings->value ( "paths/prompts" ).toString() );

	SoundDeviceList *sd=sc->getInputDevices();
	ui.cbInDevice->clear();
	settings->value ( "sounddevice/indevice" ).toString();
	for ( int i=0; i<sd->count(); i++ )
	{
		QString deviceid= ( ( SoundDevice ) sd->at ( i ) ).getDeviceID();
		ui.cbInDevice->addItem ( ( ( SoundDevice ) sd->at ( i ) ).getName(),deviceid );
		//if (deviceid.toInt()==defindevice) ui.cbInDevice->setCurrentIndex(ui.cbInDevice->count());
	}

	sd=sc->getOutputDevices();
	ui.cbOutDevice->clear();
	int defoutdevice=settings->value ( "sounddevice/outdevice" ).toInt();
	for ( int i=0; i<sd->count(); i++ )
	{
		QString deviceid= ( ( SoundDevice ) sd->at ( i ) ).getDeviceID();
		ui.cbOutDevice->addItem ( ( ( SoundDevice ) sd->at ( i ) ).getName(),deviceid );
		if ( deviceid.toInt() ==defoutdevice ) ui.cbOutDevice->setCurrentIndex ( ui.cbOutDevice->count() );
	}

	ui.cbSaveAllRecordings->setCheckState ( ( settings->value ( "saveallrecordings" ).toBool() ) ? Qt::Checked : Qt::Unchecked );
	ui.leSaveRecordingsTo->setText ( settings->value ( "pathtosaverecordings" ).toString() );


	refreshDeviceCapabilities();

	int channel=settings->value ( "sounddevice/channel" ).toInt();
	int j=0;
	while ( ( j<ui.cbChannels->count() ) && ( ui.cbChannels->itemData ( j ) !=channel ) )
	{
		j++;
	}
	if ( j==ui.cbChannels->count() )
	{
		QMessageBox::critical ( this,tr("Lesen der Kanäle fehlgeschlagen"),tr("Beim Auslesen der Kanäle aus der Konfigurationsdatei ist ein Fehler aufgetreten" ));
		Logger::log(tr("[ERR] Konnte die Kanäle nicht lesen"));
		return;
	}
	ui.cbChannels->setCurrentIndex ( j );

	QString desiredSRate=settings->value ( "sounddevice/samplerate" ).toString();
	for ( int i=0; i< ui.cbSampleRate->count(); i++ )
	{
		if ( ui.cbSampleRate->itemText ( i ) == desiredSRate )
		{
			ui.cbSampleRate->setCurrentIndex ( i );
			break;
		}
	}
	ui.hsMic->setValue ( settings->value ( "sounddevice/volume" ).toInt() );
	QStringList hosts;
	hosts= ( settings->value ( "network/juliusdaddresses" ).toString() ).split ( ";",QString::KeepEmptyParts,Qt::CaseSensitive );
	for ( int i=0; i<hosts.count(); i++ )
	{
		ui.cbAddress->addItem ( hosts[i] );
	}
	ui.cbAddress->setCurrentIndex ( ui.cbAddress->findText ( settings->value ( "network/defaultjuliusdAddress" ).toString(),Qt::MatchCaseSensitive ) );


	initCommands();
}


void SettingsView::saveCommands()
{
	Logger::log(tr("[INF] Speichere Kommandos..."));
	commandLoader->save();
}

/**
 * \brief Loads the commands from the corresponding XML file and inserts them into the list for configuring the available comamnds
 * \author Peter Grasch, Tschernegg Susanne
 */
void SettingsView::initCommands(QString path)
{
	Logger::log(tr("[INF] Importiere Kommandos von ")+path);
	commandLoader->load(path);
	CommandList commands = commandLoader->getCommands();
	
	ui.twCommand->setRowCount(commands.count());
	QTableWidgetItem *tmp;
	
	//baue combobox
	
	Logger::log(tr("[INF] Habe ")+QString::number(commands.count())+tr("Kommandos gefunden"));
	for (int i=0; i < commands.count(); i++)
	{
		tmp = new QTableWidgetItem(commands.at(i)->getName());
		ui.twCommand->setItem(i, 0, tmp);
		
		CommandType ctype = commands.at(i)->getType();
		QString strType;
			
		if (ctype ==place)
			strType = tr("Orte");
		else if (ctype == type)
			strType = tr("Sonderzeichen");
		else strType = tr("Programme");
		
		//tmp = new QTableWidgetItem(strType);
		
		//ui.twCommand->setItem(i, 1, tmp);
		
		//QComboBox *cbType = new QComboBox();
		//cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
		//cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
		//cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
		
		//cbType->setCurrentIndex(ctype);
		tmp = new QTableWidgetItem(strType);
		ui.twCommand->setItem(i, 1, tmp);
		//ui.twCommand->setCellWidget(i, 1, cbType);
		ui.twCommand->resizeColumnToContents(1);
		
		tmp = new QTableWidgetItem(commands.at(i)->getValue());
		ui.twCommand->setItem(i, 2, tmp);
	}
}

void SettingsView::apply()
{
	Logger::log(tr("[INF] Wende Settings an..."));
	
	settings->setValue ( "simonautostart",ui.cbStartSimonOnBoot->checkState() ==Qt::Checked );
	settings->setValue ( "juliusdautostart",ui.cbStartJuliusdOnBoot->checkState() ==Qt::Checked );
	settings->setValue ( "juliusdrequired",ui.cbStartJuliusAsNeeded->checkState() ==Qt::Checked );
	settings->setValue ( "askbeforeexit",ui.cbAskBeforeExit->checkState() ==Qt::Checked );
	settings->setValue ( "setsaveallrecordings",ui.cbSaveAllRecordings->checkState() ==Qt::Checked );
	settings->setValue ( "sounddevice/channel",ui.cbChannels->itemData ( ui.cbChannels->currentIndex(),Qt::UserRole ).toString() );
	settings->setValue ( "sounddevice/samplerate",ui.cbSampleRate->currentText() );
	settings->setValue ( "sounddevice/volume",ui.hsMic->sliderPosition() );
	settings->setValue ( "sounddevice/indevice",ui.cbInDevice->currentText() );
	settings->setValue ( "sounddevice/outdevice",ui.cbOutDevice->currentText() );

	settings->setValue ( "paths/lexicon",ui.leLexicon->text() );
	settings->setValue ( "paths/grammar",ui.leGrammar->text() );
	settings->setValue ( "paths/vocabul",ui.leVocab->text() );
	settings->setValue ( "paths/prompts",ui.lePrompts->text() );
	settings->setValue ( "paths/saverecordings",ui.leSaveRecordingsTo->text() );

	QString addresses="";
	for ( int i=0; i<ui.cbAddress->count(); i++ )
	{
		if ( addresses=="" ) addresses=ui.cbAddress->itemText ( i );
		else addresses=addresses+";"+ui.cbAddress->itemText ( i );

	}
	settings->setValue ( "network/juliusdaddresses",addresses );
	settings->setValue ( "network/defaultjuliusdaddress",ui.cbAddress->currentText() );

	settings->sync();
	
	
	QWidget *tmpWidget = new QWidget();
	tmpWidget = ui.twCommand->cellWidget(ui.twCommand->currentRow(), 1);
	if(tmpWidget!=NULL)
	{
		QComboBox *cbType = (QComboBox*)tmpWidget;
		QString type = cbType->itemText(cbType->currentIndex());
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText(type);
		ui.twCommand->removeCellWidget(ui.twCommand->currentRow(), 1);
		ui.twCommand->setItem(ui.twCommand->currentRow(), 1, tmp);
		
		int typeInt;
		if(type == "Orte")
			typeInt = 1;
		else if (type == "Sonderzeichen")
			typeInt = 2;
		else typeInt = 0;

		Command *newCommand = new Command(ui.twCommand->item(ui.twCommand->currentRow(),0)->text(), CommandType(typeInt), ui.twCommand->item(ui.twCommand->currentRow(),2)->text());
		if(commandLoader->commandExists(ui.twCommand->item(ui.twCommand->currentRow(),0)->text()))
			commandLoader->replaceCommand(ui.twCommand->item(ui.twCommand->currentRow(),0)->text(), newCommand);
		else commandLoader->addCommand(newCommand);
	}
	commandLoader->save();
}




/**********************************************/
/******************Tab Stuff*******************/
/**********************************************/

/**
 * \brief Untoggles all the "tab" buttons on top
 *
 * \author Peter Grasch
 */
void SettingsView::unsetAllTabs()
{
	ui.pbSystemSettings->setChecked ( false );
	ui.pbSoundSettings->setChecked ( false );
	ui.pbCommandSettings->setChecked ( false );
	ui.pbProtocolSettings->setChecked ( false );
	ui.pbRevert->setChecked ( false );

}

void SettingsView::switchToSystem()
{
	unsetAllTabs();
	ui.pbSystemSettings->setChecked ( true );
	ui.swSettings->setCurrentIndex ( 0 );

}

/**
 * \brief Switches to the "sound" tab
 *
 * \author Peter Grasch
 */
void SettingsView::switchToSound()
{
	unsetAllTabs();
	ui.pbSoundSettings->setChecked ( true );
	ui.swSettings->setCurrentIndex ( 1 );

}

/**
 * \brief Switches to the "command" tab
 *
 * \author Peter Grasch
 */
void SettingsView::switchToCommands()
{
	unsetAllTabs();

	ui.pbCommandSettings->setChecked ( true );
	ui.swSettings->setCurrentIndex ( 2 );
}

/**
 * \brief Switches to the "command" tab
 *
 * \author Peter Grasch, Phillip Goriup
 */
void SettingsView::switchToProtocols()
{
	unsetAllTabs();
	
	ui.pbProtocolSettings->setChecked ( true );
	ui.swSettings->setCurrentIndex ( 3 );
	
	QCoreApplication::processEvents();
	
	insertEntries(getEntries(NULL));

}	

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void SettingsView::onlyDay()
{
	//QMessageBox::information(this, "BLA","only Day");
	if(ui.cbOnlyDay->checkState () == Qt::Checked)
	{
		//QMessageBox::information(this, "BLA","OnlyDay in  "+ui.cwLogDay->selectedDate().toString());
		insertEntries(getEntries(&ui.cwLogDay->selectedDate()));
	}
	else
	{
		insertEntries(getEntries(NULL));
	}
}


/**
 * \brief 
 *
 * \author Phillip Goriup
 */
LogEntryList SettingsView::getEntries(QDate *day)
{	
	ui.pbLogLoad->setMaximum(0);
	ui.lbLogLoad->setText(tr("Lade Einträge..."));
	QCoreApplication::processEvents();
	ui.twLogEntries->clear();
	if(day == NULL)
	{	
		return manager->getAll();
	}
	else
	{
		return manager->getDay(*day);
	}
	// * * QMessageBox::information(this, "BLA",entries[0]->getDate().toString("yyyy/MM/dd"));
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void SettingsView::insertEntries(LogEntryList entries)
{
	QList<QTreeWidgetItem *> items;
	QDate currentdate; 
	int i = 0;
	QTreeWidgetItem *item;
	
	ui.pbLogLoad->setMaximum(entries.count());
	ui.lbLogLoad->setText(tr("Füge Einträge ein..."));
	QCoreApplication::processEvents();
	//ui.twLogEntries->hide();
	while (i < entries.count())
	{
		if (entries[i]->getDate() != currentdate)
		{
			currentdate = entries[i]->getDate();
			item = new QTreeWidgetItem(ui.twLogEntries);
			item->setText(0, currentdate.toString("yyyy/MM/dd"));
		}
		QTreeWidgetItem *child = new QTreeWidgetItem(item);
		child->setText(0,entries[i]->getTime().toString());
		child->setText(1,entries[i]->getMessage());
		ui.pbLogLoad->setValue(i);
		i++;
	}
	ui.pbLogLoad->setMaximum(1);
	ui.pbLogLoad->setValue(ui.pbLogLoad->maximum());
	ui.lbLogLoad->setText(tr("Fertig"));
	//ui.twLogEntries->show();
}




/**
 * \brief Switches to the "history" tab
 *
 * \author Peter Grasch
 */
void SettingsView::switchToHistory()
{
	unsetAllTabs();

	ui.pbRevert->setChecked ( true );
	ui.swSettings->setCurrentIndex ( 4 );
}

/**
*\author SusanneTschernegg
*/
void SettingsView::editCommand()
{
	int row = ui.twCommand->currentRow();
	//QTableWidgetItem *tmp = new QTableWidgetItem();
	QString typeStr = ui.twCommand->item(row,1)->text();
	
	QComboBox *cbType = new QComboBox();
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
	
	int posCb;
	if(typeStr == "Orte")
		posCb = 1;
	else if (typeStr == "Sonderzeichen")
		posCb = 2;
	else posCb = 0;
	cbType->setCurrentIndex(posCb);
	ui.twCommand->setCellWidget(row, 1, cbType);
	
	ui.twCommand->selectColumn(0);
	ui.twCommand->selectRow(row);
	ui.twCommand->editItem(ui.twCommand->item(row, 0));
}

/**
* \author Susanne Tschernegg
*/
void SettingsView::newCommand()
{
	int rows = ui.twCommand->rowCount();
	ui.twCommand->insertRow(rows);
	ui.twCommand->setCurrentCell(rows, 0);
	QTableWidgetItem *tmp = new QTableWidgetItem();
	ui.twCommand->setItem(rows, 0, tmp);
	ui.twCommand->editItem(tmp);
	QComboBox *cbType = new QComboBox(ui.twCommand);
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));

	//cbType->setCurrentIndex();
	ui.twCommand->setCellWidget(rows, 1, cbType);
	
	ui.twCommand->setItem(rows, 2, new QTableWidgetItem());
}

/**
* \author Susanne Tschernegg
*/
void SettingsView::deleteCommand()
{
	commandLoader->deleteCommand(ui.twCommand->item(ui.twCommand->currentRow(), 0)->text());
	ui.twCommand->removeRow(ui.twCommand->currentRow());
}

/**
* \author Susanne Tschernegg
*/
void SettingsView::activateCb()
{
	QComboBox *cbType = new QComboBox();
	cbType->addItem(QIcon(":/images/icons/emblem-system.svg"),QApplication::translate("RunDialog", "Programme", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/folder.svg"), QApplication::translate("RunDialog", "Orte", 0, QApplication::UnicodeUTF8));
	cbType->addItem(QIcon(":/images/icons/format-text-bold.svg"),QApplication::translate("RunDialog", "Sonderzeichen", 0, QApplication::UnicodeUTF8));
	ui.twCommand->setCellWidget(ui.twCommand->currentRow(), 1, cbType);
}

/**
* \author Susanne Tschernegg
*/
void SettingsView::deactivateCb(int currRow, int currCol, int prevRow, int prevCol)
{
	if(currRow == prevRow)
		return;
	//if(ui.twCommand->item(prevRow,0) && ui.twCommand->item(prevRow,2) )
	if(prevRow < 0)
		return;
	if((ui.twCommand->item(prevRow, 0)->text()==NULL)||(ui.twCommand->item(prevRow, 0)->text()==NULL)||
		(ui.twCommand->item(prevRow,0)->text().trimmed()=="")||(ui.twCommand->item(prevRow,2)->text().trimmed()==""))
	{
		int result = QMessageBox::question(this, tr("Leeres Kommandofeld"), tr("Dieses Kommando wurde nicht vollständig ausgefüllt.\nJedes Kommando muss einen Namen, Wert und Typ besitzen.\n\nWollen Sie diesen Eintrag jetzt löschen? (Klicken Sie Nein, um das Kommando jetzt zu vervollständigen)"),
			QMessageBox::Yes | QMessageBox::No | QMessageBox::Cancel);

		 if (result == QMessageBox::Yes) 
		 {
			 ui.twCommand->removeRow(prevRow);
		 } else
		 {
			ui.twCommand->selectColumn(0);
			ui.twCommand->selectRow(prevRow);
			ui.twCommand->editItem(ui.twCommand->item(prevRow, 0));
		 }
		 return;
	 }
	
	QWidget *tmpWidget = new QWidget();
	tmpWidget = ui.twCommand->cellWidget(prevRow, 1);
	if(tmpWidget!=NULL)
	{
		QComboBox *cbType = (QComboBox*)tmpWidget;
		QString type = cbType->itemText(cbType->currentIndex());
		QTableWidgetItem *tmp = new QTableWidgetItem();
		tmp->setText(type);
		ui.twCommand->removeCellWidget(prevRow, 1);
		ui.twCommand->setItem(prevRow, 1, tmp);
		
		int typeInt;
		if(type == "Orte")
			typeInt = 1;
		else if (type == "Sonderzeichen")
			typeInt = 2;
		else typeInt = 0;

		Command *newCommand = new Command(ui.twCommand->item(prevRow,0)->text(), CommandType(typeInt), ui.twCommand->item(prevRow,2)->text());
		if(commandLoader->commandExists(ui.twCommand->item(prevRow,0)->text()))
			commandLoader->replaceCommand(ui.twCommand->item(prevRow,0)->text(), newCommand);
		else commandLoader->addCommand(newCommand);
	}
}

/**
* \author Susanne Tschernegg
*/
//todo: an fixen pfad angeben
void SettingsView::reloadCommands()
{
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount(0);
	initCommands();
}

/**
* \author Susanne Tschernegg
*/
void SettingsView::showOnlyCommands()
{
	QString currType = ui.cbShowCommand->currentText();
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount(0);
	QTableWidgetItem *tmp;
	CommandList commands = commandLoader->getCommands();
	int counter = 0;
	for (int i=0; i < commands.count(); i++)
	{
		CommandType ctype = commands.at(i)->getType();
		QString strType;
			
		if (ctype ==place)
			strType = tr("Orte");
		else if (ctype == type)
			strType = tr("Sonderzeichen");
		else strType = tr("Programme");
				
		if((strType==currType)||(currType=="Alles"))
		{
			counter ++;
			ui.twCommand->setRowCount(counter);
			
			tmp = new QTableWidgetItem(commands.at(i)->getName());
			ui.twCommand->setItem(counter-1, 0, tmp);
			
			tmp = new QTableWidgetItem(strType);
			ui.twCommand->setItem(counter-1, 1, tmp);
			
			tmp = new QTableWidgetItem(commands.at(i)->getValue());
			ui.twCommand->setItem(counter-1, 2, tmp);
		}
	}
	ui.twCommand->resizeColumnToContents(1);
}

/**
* \author Susanne Tschernegg
*/
void SettingsView::clearSearchLineEdit()
{
	ui.leSearchCommand->clear();
}

/**
* \author Susanne Tschernegg
*/
void SettingsView::searchCommandList()
{
	QString searchText = ui.leSearchCommand->text();
	ui.twCommand->clearContents();
	ui.twCommand->setRowCount(0);
	QTableWidgetItem *tmp;
	CommandList commands = commandLoader->getCommands();
	int counter = 0;
	for (int i=0; i < commands.count(); i++)
	{	
		QString name = commands.at(i)->getName();
		if(name.indexOf(searchText)>=0)
		{
			counter ++;
			ui.twCommand->setRowCount(counter);
			
			tmp = new QTableWidgetItem(commands.at(i)->getName());
			ui.twCommand->setItem(counter-1, 0, tmp);
			
			CommandType ctype = commands.at(i)->getType();
			QString strType;
				
			if (ctype ==place)
				strType = tr("Orte");
			else if (ctype == type)
				strType = tr("Sonderzeichen");
			else strType = tr("Programme");
			
			tmp = new QTableWidgetItem(strType);
			ui.twCommand->setItem(counter-1, 1, tmp);
			
			tmp = new QTableWidgetItem(commands.at(i)->getValue());
			ui.twCommand->setItem(counter-1, 2, tmp);
		}
	}
	ui.twCommand->resizeColumnToContents(1);
}

/**
 * \brief Destructor
 *
 * \author Peter Grasch
 */
SettingsView::~SettingsView()
{}
