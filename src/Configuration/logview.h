//
// C++ Interface: logview
//
// Description: 
//
//
// Author: Peter Grasch <bedahr@gmx.net>, (C) 2007
//
// Copyright: See COPYING file that comes with this distribution
//
//
#ifndef LOGVIEW_H
#define LOGVIEW_H


#include "systemwidget.h"
#include "ui_logmanagerdlg.h"
#include "../SimonLib/Logging/logmanager.h"


/**
 * \class LogView
 * \brief Derived from the systemwidget class to display and search the logs
 * \version 0.1
 * \date 23.8.2007
 * \author Peter Grasch
*/
class LogView : public SystemWidget
{
	Q_OBJECT
	
private: 
	Ui::LogManagerDlg ui;
	Dates daysInLog;
	QDate clickedDate;

	LogManager *manager;
	bool abortInsertingEntries;

public slots:
	//to comply with the systemwidget
	bool apply() { return true; }
	bool init();
	bool reset() { return true; }

	void logReadFinished(int exitCode);

	void readLog();
	void startLogLoad();
	void viewAll();
	void viewDay(QDate day);

	void abort();
	void reload();

	//GUI stuff
	void displayCancel();
	void displayReload();
	void enableWidgets(bool enabled=true);
	void setStatus(QString status);
	void setProgress(int progress);
	void selectDay(QDate day);


	//inserting entries
	void displayDay(QDate day);
	void displayDay(QTreeWidgetItem *item);

	void displayDates(Dates daysAvailable);
	void completeDates();
	void processEntries(LogEntryList *entries, bool copy=false);
	void processDates(Dates daysAvailable);
	void insertEntries(LogEntryList *entries);


	//cleaning the list
	void clean();
	void cleanAllChilds();
	void deleteItem(int index);
	void deleteItem(QTreeWidgetItem *item);
	void deleteChilds(QTreeWidgetItem *item);
	void deleteChilds(int parentIndex);
	LogEntryList* filterFor(LogEntryList* log, bool copy,
				const QString filter, const short type);

	

signals:
	void stopLogRead();
	
public:
    LogView(QWidget* parent);
    bool isComplete();
    ~LogView();

};

#endif
