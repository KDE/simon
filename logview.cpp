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
#include <QRegExp>
#include <QMessageBox>
#include <QDebug>

/**
 * \brief Constrictor
 *
 * \author Phillip Goriup
 */
LogView::LogView(QWidget* parent): SystemWidget(tr("Protokoll"), QIcon(":/images/icons/text-editor.svg"), tr("Hier können Sie die letzten Aktionen von simon überprüfen"), parent)
{
	ui.setupUi(this);
	connect(ui.twLogEntries,SIGNAL(itemClicked (QTreeWidgetItem* , int)), 
		this,SLOT(insertSelectedItem(QTreeWidgetItem*)));
	connect (ui.gbOnlyDay, SIGNAL (clicked(bool)), this, SLOT (onlyDay(bool)));
	connect ( ui.cwLogDay, SIGNAL (selectionChanged()), this, SLOT (onlyDay()));
	connect ( ui.pbAbort, SIGNAL (clicked (bool)),this, SLOT(abort())); 
	connect (ui.leSearchLogs, SIGNAL(returnPressed ()), this, SLOT(search()));
	connect (ui.pbLogSearch, SIGNAL(clicked()),this,SLOT(search()));
	this->AbortFlag = true;
	this->manager = new LogManager();
	connect (this, SIGNAL(logReadStop(bool)),manager, SLOT(stop(bool)));
	
}


/**
 * \brief starts the logreading
 * this methode is called on entering the Protocol tab
 *
 * \author Phillip Goriup
 */
void LogView::enter()
{
	if (!this->manager->readLog())
	{		
		QMessageBox::critical(this,tr("Fehler beim Auslesen des Logs"),tr("Beim Auslesen der Logdatei ist ein Fehler aufgetreten.\n\nÜberprüfen Sie ob Sie die benötigten Berechtigugnen besitzen."));
	}
	else
		startLogRead();
}

/**
 * \brief Handling Methode from the Button Abort/Reload
 * handles the Button Abort/Reload and starts required  methods
 * Abort: aborts the loading of the Logfile and the loading of the TreeWidgetItems
 * Reload: reloads the Logfile or the TreeWidgetItems
 * 
 * \author Phillip Goriup
 */
void LogView::abort()
{
	displayReload();
	ui.lbLogLoad->setText("Laden Abgebrochen");
	if (dynamic_cast<QPushButton*>(sender()))
	{		
		if(this->AbortFlag)
		{
			this->AbortFlag = false;
			if(this->manager->isRunning())
				emit this->logReadStop(false);
			else
				changeLogReadFlag(false);
		}
		else
		{
			this->AbortFlag = true;
			if (!this->ui.gbOnlyDay->isChecked())
			{
				if(!this->manager->hasFinishedReading())
					this->startLogRead();
				else
				{	
					changeLogReadFlag(true);
					onlyDay(false);
				}
			}
			else
				onlyDay(true);
		}
	}
	else
	{
		emit this->logReadStop(true);
		ui.twLogEntries->clear();
	}
}

/**
 * \brief destructor
 *
 * \author Phillip Goriup
 */
LogView::~LogView()
{
	
}

/**
 * \brief 
 *
 * \author Peter Grasch
 */
bool LogView::apply()
{
	return true;
}

/**
 * \brief 
 *
 * \author Peter Grasch
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
 * \author Peter Grasch
 */
bool LogView::reset()
{
	return true;
}


/**
 * \brief inserts a selected item into the QTreeWidget
 * clears all items in the QTreeWidget that have childs and that are not the same as the currentItem
 * if the selected item is the already filled item, the methode expands or collapses the childs
 * if the selected item is not the already filled item, the methode reads out the childs and saves them into the TreeWidget
 * 
 * \author Phillip Goriup
 * @param item links to the memory of the selected Item
 * 	      column: is the index of the clicked column of the item 
 */
void LogView::insertSelectedItem(QTreeWidgetItem* item)
{
	if (ui.twLogEntries->isItemExpanded(item))
	{
		ui.twLogEntries->collapseItem(item);
		return;
	}
	int index = this->ui.twLogEntries->indexOfTopLevelItem(item);
	if (!item->parent())
	{
		displayCancel();
		this->AbortFlag = true;
		enableWidgets(false);
		this->ui.pbAbort->setEnabled(false);
		ui.lbLogLoad->setText(tr("Füge Einträge ein..."));
		QCoreApplication::processEvents();

		//remove childs from all toplevelitems
		for (int i=0; i < ui.twLogEntries->topLevelItemCount(); i++)
		{
			QList<QTreeWidgetItem*> childs = ui.twLogEntries->topLevelItem(i)->takeChildren();
			for (int j=0; j < childs.count(); j++)
				delete childs.at(j);
		}


		this->ui.pbAbort->setEnabled(true);
		

		QDate day = QDate::fromString(item->text(0),"yyyy/MM/dd");
		LogEntryList *temp = manager->getDay(day);
		LogEntryList *entries  = searchEntries(temp);
		delete temp;


		if (ui.twLogEntries->isItemExpanded(item))
		{
			ui.twLogEntries->collapseItem(item);
			
		}
		if (entries->count() != 0)
		{
			if ((item->childCount () == 0)
				|| (entries->count() != ui.twLogEntries->topLevelItem(index)->childCount ()))
			{
				deleteItem(index);
				insertChilds(entries, item, index);
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


		delete(entries);

		displayReload();
		this->AbortFlag = false;
		enableWidgets();
	}
}

/**
 * \brief deletes a item in the TreeWidget
 *
 * \author Phillip Goriup
 * @param index the index of the item that should be deleted
 */
void LogView::deleteItem(int index)
{
	QList<QTreeWidgetItem*> childs = ui.twLogEntries->topLevelItem(index)->takeChildren();
	for (int j=0; j < childs.count(); j++)
		delete childs.at(j);
	delete ui.twLogEntries->takeTopLevelItem(index);
}


/**
 * \brief Enables/disables the widgets to the given state
 * \author Peter Grasch
 * @param enabled Should we en- or disable them?
 */
void LogView::enableWidgets(bool enabled)
{
	this->ui.leSearchLogs->setEnabled(enabled);
	this->ui.cbLogError->setEnabled(enabled);
	this->ui.cbLogInfo->setEnabled(enabled);
	this->ui.cbLogUpdate->setEnabled(enabled);
	this->ui.cbLogSettings->setEnabled(enabled);

	this->ui.gbOnlyDay->setEnabled(enabled);
	qDebug() << ((enabled) ? "Onlyday wird enabled!" : "Onlyday wird net enabled!");
	this->ui.cwLogDay->setEnabled(enabled && ui.gbOnlyDay->isChecked());

	this->ui.pbActionLog->setEnabled(enabled);
	this->ui.pbRecordings->setEnabled(enabled);
	this->ui.twLogEntries->setEnabled(enabled);
	this->ui.pbLogSearch->setEnabled(enabled);
	this->ui.pbClearSearch->setEnabled(enabled);
}

/**
 * \brief starts the Read from the Logfile
 *
 * \author Phillip Goriup
 */
void LogView::startLogRead()
{
	this->LogFlag = true;
	displayCancel();
	this->AbortFlag = true;
	ui.pbLogLoad->setMaximum(0);
	ui.lbLogLoad->setText(tr("Lese Log..."));
	QCoreApplication::processEvents();

	enableWidgets(false);
	disconnect(manager,0,0,0);//disconnect everything
	connect(manager,SIGNAL(logReadFinished(int)),this,SLOT(logReadFinished(int)));
	connect(this,SIGNAL(logReadStop(bool)),this->manager,SLOT(stop(bool)));
	this->manager->start();
}

/**
 * \brief works if the Log-reading is finished
 *
 * \author Phillip Goriup
 * @param value = 1: sends out a Message, that the logread has finished
 *             value = 0: aborts the logread
 */
void LogView::logReadFinished(int value)
{
	if(value == 1)
	{
		ui.lbLogLoad->setText(tr("Log gelesen"));
		ui.pbLogLoad->setMaximum(100);
		ui.pbLogLoad->setValue(100);
		disconnect(manager,SIGNAL(logReadFinished(int)),this,SLOT(logReadFinished(int)));
		LogEntryList *temp = searchEntries(getEntries(QDate()));
		insertEntries(temp,NULL);
		delete temp;
	}
}

/**
 * \brief decides if it should be insert only a day, or all Entries into the TreeWidget 
 *
 * \author Phillip Goriup
 * @param enable = true: calls a methode that only insert a one day 
 *             enable = false: calls a methode that inserts all entries
 */
void LogView::onlyDay(bool enable)
{
	displayCancel();
	this->AbortFlag = true;
	if(enable)
	{
		LogEntryList *list = getEntries(ui.cwLogDay->selectedDate());
		if (list->count() ==0)
		{
			this->AbortFlag = false;
			displayReload();
			this->ui.lbLogLoad->setText(tr("Keine Einträge vorhanden"));
		}
		else
			insertEntries(searchEntries(list),true);
		delete list;
	}
	else
	{
		LogEntryList *temp = searchEntries(getEntries(QDate()));
		insertEntries(temp,false);
		delete temp;
	}
}

void LogView::displayCancel()
{
	this->ui.pbAbort->setText(tr("Abbrechen"));
	ui.pbAbort->setIcon(QIcon(":/images/icons/emblem-unreadable.svg"));
}

void LogView::displayReload()
{
	this->ui.pbAbort->setText(tr("Neu laden"));
	this->ui.pbLogLoad->setMaximum(100);
	this->ui.pbLogLoad->setValue(100);
	ui.pbAbort->setIcon(QIcon(":/images/icons/view-refresh.svg"));

}


/**
 * \brief returns a vector of entries
 *
 * \author Phillip Goriup
 * @param day the day that should be returned
 *             if its NULL, all entries returns
 */
LogEntryList* LogView::getEntries(QDate day)
{	
	
	ui.pbLogLoad->setMaximum(0);
	ui.lbLogLoad->setText(tr("Lade Einträge..."));
	QCoreApplication::processEvents();
	ui.twLogEntries->clear();
	if(day.isNull())
	{
		return manager->getAll();
	}
	else
	{
		return manager->getDay(day);
	}
}

/**
 * \brief inserts a vector of given entries into the QTreeWidtet
 * decide if the entries includes only one day or more days
 *
 * \author Phillip Goriup
 * @param entries: link to a memory with a list of entries
 
*             day: if its true entries includes only one day
 */
void LogView::insertEntries(LogEntryList* entries, bool day)
{
	this->ui.twLogEntries->clear();
	QDate currentdate; 
	int i = 0;
	QTreeWidgetItem *item = 0;
	
	ui.pbLogLoad->setMaximum(entries->count());
	ui.lbLogLoad->setText(tr("Füge Einträge ein..."));
	enableWidgets(false);
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

	enableWidgets();
	ui.lbLogLoad->setText(tr("Fertig"));
	displayReload();
	this->AbortFlag = false;
}

/**
 * \brief take the given Data from the GUI and calls a methode to filter the list 
 *
 * \author Phillip Goriup
 * @param list: the list that should be filtered
 */
LogEntryList* LogView::searchEntries(LogEntryList* list)
{
	int error = 0;
	int info = 0;
	int update = 0;
	int settings = 0;
	QDate date;

	if(!list)
		return new LogEntryList;

	this->LogFlag = true;
	displayCancel();
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
	

	return filterEntries(this->ui.leSearchLogs->text(), (error|info|update|settings), list);

}

/**
 * \brief filters the Entries with the given parameters
 *
 * \author Phillip Goriup
 * @param key: the text that should be searched in the entries
 *             error: decides if only errors should be taken
 *             info: decides if only infos should be taken
 *             update: decides if only unpdates should be taken
 *             settings: decides if only settings should be taken
 *             list: the list that should filtered
 */
LogEntryList* LogView::filterEntries(QString key, int categories, LogEntryList *list)
{
	int i = 0;
 	LogEntryList *FoundEntries = new LogEntryList;
	while (i<list->count())
	{
		if (((categories == 0) ||  /* If there is no selection made (we want to include everything)... */
				//case-insensitive search disabled out of performance consideration
			((categories & list->at(i).getType()) != 0)) && 
					(list->at(i).getMessage().contains(key.toLatin1())))
					//OR our search parameters include the type of the current entry...
		{
			FoundEntries->append(list->at(i));	//we add it to our found list
		}
		i++;
	}
 	return FoundEntries;
}

/**
 * \brief inserts the given entries into the TreeWidget
 * 
 * \author Phillip Goriup
 * @param entries the list of entries
 * \param item the selected item (item = null, if only a day should be inserted)
 * \param index the index where the item should be inserted
 */
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
		enableWidgets();
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

/**
 * \brief SLOT for the pressEnter Event, and for the search Button
 *
 * \author Phillip Goriup
 */
void LogView::search()
{
	if (this->ui.gbOnlyDay->isChecked())
		onlyDay(true);
	else
		onlyDay(false);
}
