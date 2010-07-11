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

#include "logview.h"

#include <QTableWidget>
#include <QTableWidgetItem>
#include <KComboBox>
#include <QCoreApplication>
#include <QRegExp>
#include <KLocale>

/**
 * \brief Constructor
 * \author Peter Grasch
 * @param parent Parent of the systemwidget
 */
LogView::LogView(QWidget* parent):QWidget(parent) // SystemWidget(i18n("Protokoll"), KIcon("utilities-log-viewer"), i18n("Hier können Sie die letzten Aktionen von simon überprüfen"), parent)
{
  ui.setupUi(this);

  // 	guessChildTriggers(this);

  clickedDate = QDate();
  connect (ui.gbOnlyDay, SIGNAL (toggled (bool)), ui.cwLogDay, SLOT (setEnabled(bool)));

  connect (ui.twLogEntries, SIGNAL (itemClicked(QTreeWidgetItem*, int)),
    this, SLOT (displayDay(QTreeWidgetItem*)));

  connect ( ui.cwLogDay, SIGNAL (selectionChanged()), this, SLOT (startLogLoad()));
  connect ( ui.gbOnlyDay, SIGNAL (toggled(bool)), this, SLOT (startLogLoad()));
  connect ( ui.pbAbort, SIGNAL (clicked (bool)),this, SLOT(abort()));

  connect (ui.leSearchLogs, SIGNAL(returnPressed ()), this, SLOT(startLogLoad()));
  connect (ui.pbLogSearch, SIGNAL(clicked()),this,SLOT(startLogLoad()));

  this->abortInsertingEntries = false;
  this->manager = new LogManager();
  connect (this, SIGNAL(stopLogRead()),manager, SLOT(stop()));
  connect(manager, SIGNAL(foundEntries(LogEntryList*,bool)), this,
    SLOT(processEntries(LogEntryList*,bool)));
  connect(manager, SIGNAL(daysAvailable(Dates)), this, SLOT(processDates(Dates)));
  connect(manager, SIGNAL(logReadFinished(int)), this, SLOT(logReadFinished(int)));

  ui.gbOnlyDay->setObjectName("gbOnlyDay");
  ui.twLogEntries->setObjectName("twLogEntries");

  // 	help = i18n("simon speichert im normalen Betrieb viele Loginformationen die im Nachhinein helfen können, Probleme nachzuvollziehen");

  ui.pbLogSearch->setIcon(KIcon("system-search"));
  ui.pbAbort->setIcon(KIcon("process-stop"));

  init();
}


/**
 * \brief Asks the manager to get all entries ready
 * \author Peter Grasch
 *
 * Sets the view in the "cancel" look and clears the entries;
 *
 * Calls readLog()
 *
 * The function actually asks for the list of days because we do not need to import
 * _every_ entry of _every_ day at once (RAM!)
 *
 * \see readLog() clean() displayCancel() LogManager::getDateList()
 */
void LogView::viewAll()
{
  displayCancel();
  clean();
  readLog();
  setStatus(i18n("Loading Overview"));

  manager->getDateList();
}


/**
 * \brief Asks the manager to get the logentries of the given day ready
 * \author Peter Grasch
 *
 * Sets the view in the "cancel" look without clearing the loaded entries;
 * Calls readLog() before asking the manager for a list of days with
 * LogManager::getDay()
 *
 * @param day The day to display
 * @see readLog() displayCancel() LogManager::getDay()
 */
void LogView::viewDay(QDate day)
{
  displayCancel();
  readLog();
  setStatus(i18n("Loading Day %1", day.toString("yyyy/MM/dd")));
  manager->getDay(day);
}


/**
 * \brief Clears the displayed list of entries and tells the manager to (re-) reads the logfile if needed
 * \author Peter Grasch
 *
 * (Re-)reads the logfile if the thread isn't busy and has not finished reading the current version
 * (determined by calling LogManager::hasFinishedReading)
 *
 * \see LogManager::start() clean()
 */
void LogView::readLog()
{
  if (!this->manager->hasFinishedReading() && (!this->manager->isBusy())) {
    clean();
    setStatus(i18n("Loading File..."));
    this->manager->start();
  }
}


/**
 * \brief Makes the list of displayed logentries ressamble the selected gui-options
 * \author Peter Grasch
 *
 * Looks at the selected options and starts the process of importing/parsing the file,
 * loading the day, applying the filters and displaying the list
 *
 * All this happens by calling the methods:
 * 	* viewAll()
 * 		if we just unchecked the "onlyday" option we want to display the whole list again
 * 		this is also the default!
 *   OR * viewDay(selected day of the calendar widget)
 * 		If "onlyday" is checked; We clear the list before we call it by using clean()
 *   OR * displayDay(selected item in the treewidget)
 * 		if there is a selected item in the treewidget which has no parent (i.e. top-level-item)
 *
 */
void LogView::startLogLoad()
{
  setStatus(i18n("Loading Log..."));
  clickedDate = QDate();
  if (sender() && sender()->objectName()=="gbOnlyDay" && !ui.gbOnlyDay->isChecked()) {
    viewAll();
    return;
  }
  ui.pbLogLoad->setMaximum(0);
  if (ui.gbOnlyDay->isChecked()) {                //just one day
    clean();
    viewDay(ui.cwLogDay->selectedDate());
  } else if (ui.twLogEntries->currentItem() && !ui.twLogEntries->currentItem()->parent())
  {
    displayDay(ui.twLogEntries->currentItem());

  }
  else {
    viewAll();
  }
}


/**
 * \brief Calls displayReload() if the code is non-zero
 * \author Peter Grasch
 *
 * If the logmanager is interrupted or an error occurs it emits the logReadFinished(int) signal with a
 * non-zero exit code;
 * This is caught by this slot as there will be taken no further actions (no logentries emitted, etc.);
 *
 * If you remove this the gui will just stay the same if the LogManager is interrupted during run()!
 *
 * @param code if it is non-zero we will call displayReload()
 * @see displayReload()
 */
void LogView::logReadFinished(int code)
{
  if (code != 0)
    displayReload();
  //code is != 0 when we aborted the process
}


/**
 * \brief Aborts
 * \author Peter Grasch
 *
 * This actually does pretty much nothing - it just sets the killflag "abortInsertingEntries" to true
 * and emits the stopLogRead() signal
 */
void LogView::abort()
{
  emit stopLogRead();
  abortInsertingEntries=true;
}


/**
 * \brief Reloads
 * \author Peter Grasch
 *
 * This just calls startLogLoad; It is only used to make the code more transparent (it may be more obvious
 * if we call reload() instead of startLogLoad() in certain situations)
 */
void LogView::reload()
{
  startLogLoad();
}


/////////////////////////////////////////
//	inserting entries
/////////////////////////////////////////

/**
 * \brief Overloaded function: Extracts the day from the TreeWidgetItem and calls displayDay(QDate)
 * \author Peter Grasch
 *
 * If we were called from the treewidget and the item we got is expanded we collapse it and return
 *
 * We store the date we extract in the clickedDate member variable so that we can select the date
 * again when we are finished;
 *
 * Deletes the given item (the "same" item (content-wise) will be inserted by insertEntries later on)
 *
 * @param item The QTreeWidgetItem to extract the date from
 * @see displayCancel() deleteItem()
 */
void LogView::displayDay(QTreeWidgetItem *item)
{
  if (!item) return;
  if ((sender() && (sender()->objectName() == "twLogEntries")) &&
  item->isExpanded() && item->childCount()>0) {
    item->setExpanded(false);
    deleteChildren(item);
    return;
  }

  QDate day = QDate::fromString(item->text(0), "yyyy/MM/dd");
  clickedDate = day;
  if (day.isNull()) return;
  displayCancel();

  deleteItem(item);

  displayDay(day);
}


/**
 * \brief Display the given Day
 * \author Peter Grasch
 *
 * Calls displayCancel() and removes all children from all the other entries
 *
 * @param day The day to display
 */
void LogView::displayDay(QDate day)
{
  displayCancel();
  cleanAllChildren();
  this->viewDay(day);
}


/**
 * \brief Display the available dates
 * \author Peter Grasch
 *
 * Inserts the available dates (dates only - no entries to it)
 *
 * @param daysAvailable the available dates
 */
void LogView::displayDates(Dates daysAvailable)
{
  ui.pbLogLoad->setMaximum(daysAvailable.size());
  setStatus(i18n("Parsing Days..."));
  for (int i=0; i < daysAvailable.size(); i++) {
    ui.twLogEntries->addTopLevelItem(new QTreeWidgetItem(ui.twLogEntries, QStringList() <<
      daysAvailable.at(i).toString("yyyy/MM/dd") ));
    setProgress(i);
  }
  displayReload();
}


/**
 * \brief Process the retrieved days
 * \author Peter Grasch
 *
 * Cleans the whole treewidget, stores the retrieved days in the member daysInLog and calls
 * displayDates() with the retrieved dates;
 *
 * @param daysAvailable
 */
void LogView::processDates(Dates daysAvailable)
{
  clean();
  daysInLog = daysAvailable;
  displayDates(daysAvailable);
}


/**
 * \brief Processes the retrieved logentries
 *
 * Extracts the filtersettings (searchtext, flags) from the gui and builds passes it on to the
 * filterFor() function;
 * It inserts the filtered entries (returned by filterFor) and inserts them by calling
 * insertEntries()
 *
 * Deletes the filtered list when done
 *
 * @param entries The entries to insert
 * @param copy This determines if we are working with a copy of the actual data
 * @see insertEntries() filterFor()
 */
void LogView::processEntries(LogEntryList *entries, bool copy)
{
  if(!entries) return;

  int error = 0;
  int info = 0;
  int update = 0;
  int settings = 0;
  QDate day;
  if(ui.gbOnlyDay->isChecked())
    day = ui.cwLogDay->selectedDate();

  if (this->ui.cbLogError->isChecked ())
    error = 1;
  if (this->ui.cbLogInfo->isChecked ())
    info = 2;
  if (this->ui.cbLogUpdate->isChecked ())
    update = 4;
  if (this->ui.cbLogSettings->isChecked ())
    settings = 8;

  LogEntryList *filtered =
    filterFor( entries, copy, ui.leSearchLogs->text(), (error|info|update|settings));

  insertEntries(filtered);

  delete filtered;
}


/**
 * \brief Searchs the treewidget and selects the day if it finds it
 * \author Peter Grasch
 *
 * This is mainly used to re-select the day that we "lost" when viewing the details of one day;
 * The process looks like this:
 * 	1.) The user selects the date 2007/08/23
 * 	2.) Simon deletes the 2007/08/23 item and asks the manager for entries of that day
 * 	3.) We insert them by going through processEntries() (filterFor(), insertEntries())
 * 	4.) We select the date 2007/08/23 (this is no longer selected by default as we deleted the
 * 	    corresponding item in step 2
 *
 * @param day The day that we try to select
 */
void LogView::selectDay(QDate day)
{
  QTreeWidgetItem *toSelect=0;
  QString strDate = day.toString("yyyy/MM/dd");
  for (int i=0; i < ui.twLogEntries->topLevelItemCount(); i++) {
    if (ui.twLogEntries->topLevelItem(i)->text(0) == strDate) {
      toSelect = ui.twLogEntries->topLevelItem(i);
      break;
    }
  }
  if (toSelect) ui.twLogEntries->setCurrentItem(toSelect);
}


/**
 * \brief Inserts the given entries into the treewidget
 *
 * Goes through the given entries and inserts them one-by-one into the treewidget;
 * This process can be interrupted at any time by calling abort() (which in turn sets abortInsertingEntries
 * true);
 *
 * Colors every entry in the color corresponding to the type of the entry:
 * 	* Pale yellow for information
 * 	* Red for error
 * 	* Green for update
 * 	* Blue for a changed setting
 * 	* Bright yellow for unknown
 *
 * This also checks if the date fo the first entry is the same as the one of the last - in that case
 * (as the list is sorted) we can safely assume that there was only one day inserted;
 * In that case we expand the newly inserted item;
 *
 * If we have _not_ selected onlyday, we complete the list of available dates by calling completeDates()
 * which restores deleted Days;
 *
 * Afterwards we re-select the date we selected initially (stored in clickedDate) by calling
 * selectDate(QDate);
 * This is used to solve a problem we have with search a pattern that does not exist;
 * Example:
 * 	We search for the string: "iosghxdofijvg" which is not found in any entry of the current date:
 *
 * 	1.) The user selects the date 2007/08/23
 * 	2.) Simon deletes the 2007/08/23 item and asks the manager for entries of that day
 * 	3.) At this point the focus is set to the preceding element in the TreeWidget
 * 	    (for example 2007/08/22)
 * 	3.) We insert them by going through processEntries() which filters out ALL entries with
 * 	    filterFor() (as no entry matches the pattern)
 * 	4.) The deleted day is restored by completeDates()
 *
 * As you might have noticed the focus would be still on 2007/08/22 in that example;
 * We solve that by toring 2007/08/23 in the member "clickedDate" and re-setting the focus on that date
 * after calling completeDates() by calling selectDay(QDate)
 *
 *
 * \author Peter Grasch
 * @param entries The entries to insert
 * @see completeDates() selectDay()
 */
void LogView::insertEntries(LogEntryList *entries)
{
  if(!entries) return;

  setStatus(i18n("Filling in Entries..."));
  QDate currentDay;                               //where are we now?

  QDate firstDay;                                 //store the first day
  if (entries->size() > 0) firstDay = entries->at(0).getDate();
  //that way we can determine if we inserted just one day;
  //in that case we most probably want to open it as it is the only possible reaction
  //to that scenario;

  QTreeWidgetItem *top=0;

  int i=0;
  QCoreApplication::processEvents();

  ui.pbLogLoad->setMaximum(entries->size());
  QTreeWidgetItem *newChild;
  QColor color;
  QBrush errorBrush=QBrush(QColor(255,0,0),Qt::SolidPattern);
  QBrush infoBrush=QBrush(QColor(255,252,207),Qt::SolidPattern);
  QBrush updateBrush=QBrush(QColor(60,190,80),Qt::SolidPattern);
  QBrush settingsBrush=QBrush(QColor(85,160,250),Qt::SolidPattern);
  QBrush unknownBrush=QBrush(QColor(250,250,10),Qt::SolidPattern);

  QBrush activeBrush;

  while (i<entries->size() && !abortInsertingEntries) {
    LogEntry entry=entries->at(i);
    if (currentDay != entry.getDate()) {
      top = new QTreeWidgetItem(ui.twLogEntries, QStringList() <<
        entry.getDate().toString("yyyy/MM/dd") );
      ui.twLogEntries->addTopLevelItem(top);
      ui.twLogEntries->sortItems(0,Qt::AscendingOrder);
      currentDay = entry.getDate();
    }
    newChild = new QTreeWidgetItem(QStringList()
      << entry.getTime().toString("hh:mm")
      << entry.getMessage());

    if (entry.getType() == ERR)
      activeBrush = errorBrush;
    else if (entry.getType() == INF)
      activeBrush = infoBrush;
    else if (entry.getType() == UPD)
      activeBrush = updateBrush;
    else if (entry.getType() == SET)
      activeBrush = settingsBrush;
    else
      activeBrush = unknownBrush;

    newChild->setBackground(0, activeBrush);
    newChild->setBackground(1, activeBrush);
    top->addChild(newChild);

    i++;
    if ((i % 300) == 0) setProgress(i);
  }

                                                  //we inserted just one day
  if ((firstDay == currentDay) && (entries->count() != 0)) {
    ui.twLogEntries->setCurrentItem(top);
    ui.twLogEntries->expandItem(top);
  }

  setProgress(entries->size());
  ui.twLogEntries->resizeColumnToContents(1);

  if (!ui.gbOnlyDay->isChecked()) {
    completeDates();
  }

  if (!clickedDate.isNull())
    selectDay(clickedDate);
  displayReload();
}


/**
 * \brief Completes the displayed list of dates with the member daysInLog
 * \author Peter Grasch
 *
 * Looks for dates sstored in the member "daysInLog" but not displayed in the treewidget and inserts them
 *
 * The member is most likely set when retrieving the list from the logmanager
 *
 * \see viewAll()
 */
void LogView::completeDates()
{
  Dates daysToAdd = daysInLog;
  QDate currentDay;
  for (int i=0; i < ui.twLogEntries->topLevelItemCount(); i++) {
    currentDay = QDate::fromString(ui.twLogEntries->topLevelItem(i)->text(0),
      "yyyy/MM/dd");
    for (int j=0; j < daysToAdd.count(); j++) {
      if (currentDay == daysToAdd[j])
        daysToAdd.remove(j);
    }
  }
  if (daysToAdd.count() > 0) {
    ui.pbLogLoad->setMaximum(daysToAdd.count());
    displayDates(daysToAdd);
    ui.twLogEntries->sortItems(0,Qt::AscendingOrder);
  }
}


/////////////////////////////////////////
//	cleaning the list
/////////////////////////////////////////

/**
 * \brief Deletes all children of all toplevelitems in the twLogEntries treewidget
 * \author Peter Grasch
 *
 * Calls deleteChildren() for each toplevelitem-index
 *
 * \see deleteChildren()
 */
void LogView::cleanAllChildren()
{
  for (int i=0; i < ui.twLogEntries->topLevelItemCount(); i++) {
    deleteChildren(i);
  }
}


/**
 * \brief Completele clears the displays the treewidget
 * \author Peter Grasch
 *
 * \see deleteItem()
 */
void LogView::clean()
{
  while(ui.twLogEntries->topLevelItemCount() > 0) {
    ui.twLogEntries->collapseItem(0);
    deleteItem(0);
  }
}


/**
 * \brief Overloaded function - calls deleteItem(QTreeWidgetItem*)
 * \author Peter Grasch
 * @param index The index of the item to delete
 */
void LogView::deleteItem(int index)
{
  deleteItem(ui.twLogEntries->takeTopLevelItem(index));
}


/**
 * \brief Deletes the given item completely (i.e. also removes all children)
 * \author Peter Grasch
 * @param item The item to remove
 */
void LogView::deleteItem(QTreeWidgetItem *item)
{
  deleteChildren(item);
  delete item;
}


/**
 * \brief Deletes the children of the given item
 * \author Peter Grasch
 * @param item The item to remove the children from
 */
void LogView::deleteChildren(QTreeWidgetItem *item)
{
  QList<QTreeWidgetItem*> children = item->takeChildren();
  while(children.count() > 0)
    delete children.takeAt(0);
}


/**
 * \brief Overloaded function: calls deleteChildren(QTreeWidgetItem*)
 * \author Peter Grasch
 * @param parentIndex
 */
void LogView::deleteChildren(int parentIndex)
{
  deleteChildren(ui.twLogEntries->topLevelItem(parentIndex));
}


/**
 * \brief Filters the given list of logentries by the given criterias
 * \author Peter Grasch
 *
 * If copy is set we will clean the list in-place and return a pointer of the original (but now cleaned)
 * list;
 * Else we will create a new list containing only matching entries and return a pointer of it.
 *
 * @param log The logentrylist to filter
 * @param copy Set this if you are working with a copy instead of the actual data
 * @param day Only this day
 * @param filter Only messages that contain that pattern (this is CaseSensitive!)
 * @param type Only messages of that type (ERR/INF/UPD/SET) - they are not mutual exclusive!
 * @return The filtered list
 */
LogEntryList* LogView::filterFor(LogEntryList* log, bool copy,
const QString filter, const short type)
{
  if (!log) new LogEntryList();

  LogEntryList *dest;
  if (copy) dest = log;
  else dest = new LogEntryList();

  setStatus(i18n("Filtering List..."));
  int i=0;
  ui.pbLogLoad->setMaximum(log->size());
  while (i< log->size() && !abortInsertingEntries) {
    LogEntry entry = log->at(i);
    if ((entry.getMessage().contains(filter.toLatin1()) &&
    ((type==0) || (type & entry.getType())))) {
      //is a match
      if (!copy)
        *dest << entry;
    } else if (copy)
    {
      log->removeAt(i);
      i--;
    }
    i++;
    if (i%300) setProgress(i);
  }
  return dest;
}


/////////////////////////////////////////
//	GUI
/////////////////////////////////////////

/**
 * \brief Disables the Widgets, connects to abort() instead of reload() and sets a appropriate button-text
 * \author Peter Grasch
 */
void LogView::displayCancel()
{
  enableWidgets(false);
  this->ui.pbAbort->setText(i18n("Cancel"));
  ui.pbAbort->setIcon(KIcon("process-stop"));
  disconnect(ui.pbAbort, SIGNAL(clicked()), this, SLOT(reload()));
  connect(ui.pbAbort, SIGNAL(clicked()), this, SLOT(abort()));
}


/**
 * \brief Enables the Widgets, connects to reload() instead of abort() and sets a appropriate button-text
 * \author Peter Grasch
 */
void LogView::displayReload()
{
  setStatus(i18n("Finished"));
  enableWidgets(true);
  abortInsertingEntries=false;
  this->ui.pbAbort->setText(i18n("Reload"));
  this->ui.pbLogLoad->setMaximum(100);
  this->ui.pbLogLoad->setValue(100);
  ui.pbAbort->setIcon(KIcon("view-refresh"));
  disconnect(ui.pbAbort, 0,0,0);
  connect(ui.pbAbort, SIGNAL(clicked()), this, SLOT(reload()));
}


/**
 * \brief En- / Disables all the input widgets to forbid user input (most likly when busy)
 * \author Peter Grasch
 *
 * This does not affect the cancel/reload button
 *
 * @param enabled Should we enable or disable the widgets?
 */
void LogView::enableWidgets(bool enabled)
{
  this->ui.leSearchLogs->setEnabled(enabled);
  this->ui.lbSearchLogsFor->setEnabled(enabled);
  this->ui.cbLogError->setEnabled(enabled);
  this->ui.cbLogInfo->setEnabled(enabled);
  this->ui.cbLogUpdate->setEnabled(enabled);
  this->ui.cbLogSettings->setEnabled(enabled);

  this->ui.gbOnlyDay->setEnabled(enabled);
  this->ui.cwLogDay->setEnabled(enabled && ui.gbOnlyDay->isChecked());

  this->ui.twLogEntries->setEnabled(enabled);
  this->ui.pbLogSearch->setEnabled(enabled);
  this->ui.pbClearSearch->setEnabled(enabled);
}


/**
 * \brief Sets the displayed status to the given status
 *
 * This will call QCoreApplication::processEvents()
 *
 * \author Peter Grasch
 * @param status The status to set it to
 */
void LogView::setStatus(QString status)
{
  ui.lbLogLoad->setText(status);
  QCoreApplication::processEvents();
}


/**
 * \brief Sets the Progress to the given progress
 *
 * This will call QCoreApplication::processEvents()
 *
 * \author Peter Grasch
 * @param progress The progress to set to
 */
void LogView::setProgress(int progress)
{
  ui.pbLogLoad->setValue(progress);
  QCoreApplication::processEvents();
}


/**
 * \brief Inits the controls
 * \author
 * Connects the signal shown() / hidden() to startLogLoad() / abort() resp.
 * @return
 */
bool LogView::init()
{
  connect(this, SIGNAL(shown()), this, SLOT(startLogLoad()));
  connect(this, SIGNAL(hidden()), this, SLOT(abort()));
  return true;
}


LogView::~LogView()
{
  manager->deleteLater();
}
