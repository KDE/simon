//
// C++ Implementation: logview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#include "logview.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QCoreApplication>
#include <QMessageBox>
#include <QRegExp>



/**
 * \brief 
 *
 * \author Phillip Goriup
 */
LogView::LogView(QWidget* parent): SystemWidget(tr("Protokoll"), QIcon(":/images/icons/text-editor.svg"), tr("Hier können Sie die letzten Aktionen von simon überprüfen"), parent)
{
	ui.setupUi(this);
	connect(ui.twLogEntries,SIGNAL(itemClicked (QTreeWidgetItem* , int)),this,SLOT(insertSelectedItem(QTreeWidgetItem* , int)));
	connect (ui.gbOnlyDay, SIGNAL (clicked(bool)), this, SLOT (onlyDay(bool)));
	connect ( ui.cwLogDay, SIGNAL (selectionChanged()), this, SLOT (onlyDay()));
	connect ( ui.pbAbort, SIGNAL (clicked (bool)),this, SLOT(abort())); 
	connect (ui.leSearchLogs, SIGNAL(returnPressed ()), this, SLOT(search()));
	connect (ui.pbLogSearch, SIGNAL(clicked()),this,SLOT(search()));
	this->AbortFlag = true;
	this->manager = new LogManager();
}


/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void LogView::enter()
{
	switchToProtocols();
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void LogView::abort()
{
	ui.pbLogLoad->setMaximum(100);
	ui.pbLogLoad->setValue(100);
	ui.lbLogLoad->setText("Laden Abgebrochen");
	if (dynamic_cast<QPushButton*>(sender()))
	{		
		if(this->AbortFlag)
		{
			this->ui.pbAbort->setText("Reload");
			this->AbortFlag = false;
			if(this->manager->isRunning())
			{
				emit this->logReadStop(false);
			}
			else
			{
				changeLogReadFlag(false);
			}
		}
		else
		{
			this->ui.pbAbort->setText("Abbrechen");
			this->AbortFlag = true;
			if (!this->ui.gbOnlyDay->isChecked())
			{
				if(!this->manager->hasFinishedReading())
				{
					this->startLogRead();
				}
				else
				{	
					changeLogReadFlag(true);
					onlyDay(false);
				}
			}
			else
			{
				onlyDay(true);
			}
		}
	}
	else
	{
		emit this->logReadStop(true);
		ui.twLogEntries->clear();
	}
	
	/*if (ui.gbOnlyDay->isChecked())
	{
		//disconnect (ui.gbOnlyDay, SIGNAL (stateChanged(bool)), this, SLOT (onlyDay(bool)));
		ui.gbOnlyDay->setChecked(false);
		//ui.cwLogDay->setEnabled( false );
		//connect (ui.cbOnlyDay, SIGNAL (stateChanged(int)), this, SLOT (onlyDay()));
	}*/
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
LogView::~LogView()
{
	
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
bool LogView::apply()
{
	return true;
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
bool LogView::init()
{
	connect(this, SIGNAL(shown()), this, SLOT(enter()));
	connect(this, SIGNAL(hidden()), this, SLOT(abort()));
	return true;
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
bool LogView::reset()
{
	return true;
}


/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void LogView::insertSelectedItem(QTreeWidgetItem* item, int column)
{	
	
	int index = this->ui.twLogEntries->indexOfTopLevelItem(item);
	if ((column == 0) && (!item->parent()))
	{
		this->ui.pbAbort->setText("Abbrechen");
		this->AbortFlag = true;
	
		int i = 0;
		QTreeWidgetItem *temp = new QTreeWidgetItem();
		QTreeWidgetItem *garbage = new QTreeWidgetItem();
		deactivateProtocolWidgets();
		this->ui.pbAbort->setEnabled(false);
		QCoreApplication::processEvents();
		ui.lbLogLoad->setText(tr("Füge Einträge ein..."));
		while (i<this->ui.twLogEntries->topLevelItemCount ())
		{			
			if (!this->ui.twLogEntries->topLevelItem(i)->childCount () == 0  && ui.twLogEntries->topLevelItem(i)->text(0) != item->text(0))
			{	
				garbage = this->ui.twLogEntries->takeTopLevelItem(i);
				temp->setText(0,garbage->text (0));
				this->ui.twLogEntries->insertTopLevelItem (i, temp) ;
				this->ui.twLogEntries->setCurrentItem(temp);
				delete garbage;
			}
			i++;
		}
		this->ui.pbAbort->setEnabled(true);
		
		QDate day = QDate::fromString(item->text(0),"yyyy/MM/dd");
		LogEntryList *entries  = searchEntries(manager->getDay(day));
		if (entries->count() != 0)
		{	
			if (item->childCount () == 0)
			{
				deleteItem(index);
				insertChilds(entries, item, index);
			}
			else
			{	
				if (this->ui.twLogEntries->isItemExpanded ( item))
				{
					this->ui.twLogEntries->collapseItem(item);
					if (entries->count() != this->ui.twLogEntries->topLevelItem(index)->childCount ())
					{
						deleteItem(index);
						insertChilds(entries, item, index);
					}
				}
				else
				{	
					this->ui.twLogEntries->expandItem(item);
					if (entries->count() != this->ui.twLogEntries->topLevelItem(index)->childCount ())
					{
						deleteItem(index);
						insertChilds(entries, item, index);
					}
					
				}
			}
		}
		else
		{
			QString str = this->ui.twLogEntries->topLevelItem(index)->text(0);
			deleteItem(index);
			QTreeWidgetItem *temp = new QTreeWidgetItem();
			temp->setText(0,str);
			this->ui.twLogEntries->insertTopLevelItem ( index, temp);
			this->ui.lbLogLoad->setText("Keine Einträge gefunden");
			this->ui.twLogEntries->setCurrentItem(this->ui.twLogEntries->topLevelItem(index));
		}
		ui.pbLogLoad->setMaximum(100);
		ui.pbLogLoad->setValue(ui.pbLogLoad->maximum());
		//delete(entries);
		ui.pbAbort->setText("Reload");
		this->AbortFlag = false;
		activateProtocolWidgets();
	}		
}

void LogView::deleteItem(int index)
{
	QTreeWidgetItem *garbage = new QTreeWidgetItem() ;
	garbage = this->ui.twLogEntries->takeTopLevelItem (index); 
	delete garbage;
	
	ui.lbLogLoad->setText(tr("Füge Einträge ein..."));
	deactivateProtocolWidgets();
	QCoreApplication::processEvents();
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void LogView::deactivateProtocolWidgets()
{
	this->ui.leSearchLogs->setEnabled(false);
	this->ui.cbLogError->setEnabled(false);
	this->ui.cbLogInfo->setEnabled(false);
	this->ui.cbLogUpdate->setEnabled(false);
	this->ui.cbLogSettings->setEnabled(false);
	this->ui.cwLogDay->setEnabled(true);
	this->ui.gbOnlyDay->setEnabled(false);
	this->ui.pbActionLog->setEnabled(false);
	this->ui.pbRecordings->setEnabled(false);
	this->ui.twLogEntries->setEnabled(false);
	this->ui.pbAbort->setEnabled(true);
	//this->ui.pbApply->setEnabled(false);
	//this->ui.pbConfirm->setEnabled(false);
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void LogView::activateProtocolWidgets()
{
	this->ui.leSearchLogs->setEnabled(true);
	this->ui.cbLogError->setEnabled(true);
	this->ui.cbLogInfo->setEnabled(true);
	this->ui.cbLogUpdate->setEnabled(true);
	this->ui.cbLogSettings->setEnabled(true);
	this->ui.gbOnlyDay->setEnabled(true);
	this->ui.pbActionLog->setEnabled(true);
	this->ui.pbRecordings->setEnabled(true);
	this->ui.twLogEntries->setEnabled(true);
	//this->ui.pbAbort->setEnabled(false);
	//this->ui.pbApply->setEnabled(true);
	//this->ui.pbConfirm->setEnabled(true);
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void LogView::switchToProtocols()
{
	if (!this->manager->readLog())
	{		
		QMessageBox::critical(this,tr("Fehler beim Auslesen des Logs"),tr("Beim Auslesen der Logdatei ist ein Fehler aufgetreten.\n\nÜberprüfen Sie ob Sie die benötigten Berechtigugnen besitzen."));
	}
	else
	{
		startLogRead();
	}

}


void LogView::startLogRead()
{
		this->LogFlag = true;
		ui.pbAbort->setText("Abbrechen");
		this->AbortFlag = true;
		ui.pbLogLoad->setMaximum(0);
		ui.lbLogLoad->setText(tr("Lese Log..."));
		QCoreApplication::processEvents();
	
		deactivateProtocolWidgets();
		disconnect(manager,0,0,0);//disconnect everything
		connect(manager,SIGNAL(logReadFinished(int)),this,SLOT(logReadFinished(int)));
		connect(this,SIGNAL(logReadStop(bool)),this->manager,SLOT(stop(bool)));
		this->manager->start();
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void LogView::logReadFinished(int value)
{
	if(value == 1)
	{
		//QMessageBox::critical(this,"start entries schreiben","");
		ui.lbLogLoad->setText(tr("Log gelesen"));
		ui.pbLogLoad->setMaximum(100);
		ui.pbLogLoad->setValue(100);
		disconnect(manager,SIGNAL(logReadFinished(int)),this,SLOT(logReadFinished(int)));
		insertEntries(searchEntries(getEntries(NULL)),NULL);
	}
/*	else
	{
		
	}*/
}

/**
 * \brief 
 *
 * \author Phillip Goriup
 */
void LogView::onlyDay(bool enable)
{
	this->ui.pbAbort->setText("Abbrechen");
	this->AbortFlag = true;
	if(enable)
	{
		if (getEntries(&ui.cwLogDay->selectedDate())->count() ==0)
		{
			this->ui.pbAbort->setText("Reload");
			this->AbortFlag = false;
			this->ui.lbLogLoad->setText("keine Einträge vorhanden");
			this->ui.pbLogLoad->setMaximum(100);
			this->ui.pbLogLoad->setValue(100);
		}
		else	
		{
			insertEntries(searchEntries(getEntries(&ui.cwLogDay->selectedDate())),true);
			//insertEntries(getEntries(&ui.cwLogDay->selectedDate()), true);
		}
	}
	else
	{
		//this->ui.cwLogDay->setEnabled(true);
		insertEntries(searchEntries(getEntries(NULL)),false);
	}
}


/**
 * \brief 
 *
 * \author Phillip Goriup
 */
LogEntryList* LogView::getEntries(QDate *day)
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
void LogView::insertEntries(LogEntryList* entries, bool day)
{
	this->ui.twLogEntries->clear();
	QDate currentdate; 
	int i = 0;
	QTreeWidgetItem *item = new QTreeWidgetItem();
	
	ui.pbLogLoad->setMaximum(entries->count());
	ui.lbLogLoad->setText(tr("Füge Einträge ein..."));
	QCoreApplication::processEvents();
	QColor color;
	
	
	ui.pbLogLoad->setMaximum(entries->count());
	ui.lbLogLoad->setText(tr("Füge Einträge ein..."));
	
	deactivateProtocolWidgets();
	QCoreApplication::processEvents();
	
	if(day)
	{
		insertChilds(entries, item, -1);
	}
	else
	{
		while (i < entries->count())
		{
			QCoreApplication::processEvents();
			if(!this->LogFlag)
			{
				changeLogReadFlag(true);
				break;
			}
			
			if (entries->at(i).getDate() != currentdate)
			{
				currentdate = entries->at(i).getDate();
				item = new QTreeWidgetItem(ui.twLogEntries);
				item->setText(0, currentdate.toString("yyyy/MM/dd"));
			}
			
			ui.pbLogLoad->setValue(i);
			i++;
		}
	}
	activateProtocolWidgets();
	ui.pbLogLoad->setMaximum(1);
	ui.pbLogLoad->setValue(ui.pbLogLoad->maximum());
	ui.lbLogLoad->setText(tr("Fertig"));
	ui.pbAbort->setText("Reload");
	this->AbortFlag = false;
	//delete entries;
	//ui.twLogEntries->show();
}


LogEntryList* LogView::searchEntries(LogEntryList* list)
{
	int error = 0;
	int info = 0;
	int update = 0;
	int settings = 0;
	QDate date;
	if(!list)
		list = new LogEntryList;
	this->LogFlag = true;
	ui.pbAbort->setText("Abbrechen");
	this->AbortFlag = true;
	ui.pbLogLoad->setMaximum(0);
	ui.lbLogLoad->setText(tr("Suche Einträge"));
	QCoreApplication::processEvents();
	if (this->ui.cbLogError->isChecked ())
		error = 1;
	if (this->ui.cbLogInfo->isChecked ())
		info = 2;
	if (this->ui.cbLogUpdate->isChecked ())
		update = 4;
	if (this->ui.cbLogSettings->isChecked ())
		settings = 8;

/*	if (this->ui.gbOnlyDay->isChecked ())
		list = getEntries(&this->ui.cwLogDay->selectedDate());
	else
		list = getEntries(NULL);

	//LogEntryList * temp = filterEntries(this->ui.leSearchLogs->text(), error, info, update, settings, list);
	if(!temp)
		return;*/
	return filterEntries(this->ui.leSearchLogs->text(), error, info, update, settings, list);
	//insertEntries(temp, (temp->count()<=10000) /*uhuh*/);
		
}

LogEntryList* LogView::filterEntries(QString key, int error, int info, int update, int settings, LogEntryList *list)
{
	int i = 0;
 	LogEntryList *FoundEntries = new LogEntryList;
	int searchParameter = (error | info | update | settings);
	while (i<list->count())
	{
		if (((searchParameter == 0) ||  /* If there is no selection made (we want to include everything)... */
			((searchParameter & list->at(i).getType()) != 0)) && (list->at(i).getMessage().contains(key.toLatin1())))
					//OR our search parameters include the type of the current entry...
		{
			FoundEntries->append(list->at(i));	//we add it to our found list
		}
		i++;
	}
 	return FoundEntries;
}

void LogView::insertChilds(LogEntryList* entries, QTreeWidgetItem* item, int index)
{
	QColor color;
	QDate currentdate;
	QString str;
	int i = 0;
	ui.pbLogLoad->setMaximum(entries->count());
	while (i < entries->count())
	{
		if(!this->LogFlag)
		{	
			str = item->text(0);
			if (index == -1)
				changeLogReadFlag(true);
			else
				delete(item);
			break;
		}
		QCoreApplication::processEvents();
		if (entries->at(i).getDate() != currentdate)
		{
			currentdate = entries->at(i).getDate();
			if (index == -1)
				item = new QTreeWidgetItem(ui.twLogEntries);
			else 
				item = new QTreeWidgetItem();
			item->setText(0, currentdate.toString("yyyy/MM/dd"));
		}
		QTreeWidgetItem *child = new QTreeWidgetItem(item);
		child->setText(0,entries->at(i).getTime().toString());
		child->setText(1,entries->at(i).getMessage());
		
		if (entries->at(i).getType() == ERR)
			color = QColor(255, 0, 0);
		else if (entries->at(i).getType() == INF)
			color = QColor(255, 252, 207);
		else if (entries->at(i).getType() == UPD)
			color = QColor(60, 190,80);
		else if (entries->at(i).getType() == SET)
			color = QColor(85,160,250);
		else
			color = QColor(250,250,10);
		child->setBackground(0, QBrush(color,Qt::SolidPattern));
		child->setBackground(1, QBrush(color,Qt::SolidPattern));;
		ui.pbLogLoad->setValue(i);
		i++;
	}
	ui.lbLogLoad->setText(tr("Fertig"));
	
	if (LogFlag)
	{
		this->ui.twLogEntries->insertTopLevelItem ( index, item ) ;
		this->ui.twLogEntries->setCurrentItem ( item) ;
		this->ui.twLogEntries->expandItem(item);
		activateProtocolWidgets();
	}
	else
	{
		QTreeWidgetItem *tempitem = new QTreeWidgetItem();
		tempitem->setText(0,str);
		this->ui.twLogEntries->insertTopLevelItem (index, tempitem);
		this->ui.twLogEntries->setCurrentItem (tempitem) ;
		changeLogReadFlag(true);			
	}
}
void LogView::search()
{
	if (this->ui.gbOnlyDay->isChecked())
		onlyDay(true);
	else
		onlyDay(false);
}

