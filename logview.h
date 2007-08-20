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
#include "logmanager.h"

/**
	@author Peter Grasch <bedahr@gmx.net>
*/
class LogView : public SystemWidget
{
	Q_OBJECT
	
private: 
	Ui::LogManagerDlg ui;
	
	LogManager *manager;
	bool LogFlag;
	bool AbortFlag;

public slots:
	void enter();
	void abort();
	bool apply();
	bool init();
	bool reset();

	void insertEntries(LogEntryList *entries, bool day);
	LogEntryList*  getEntries(QDate *day);
	void onlyDay(bool enable = true);
	
	void changeLogReadFlag(bool b){this->LogFlag =b;}
	void logReadFinished(int value);
	void deactivateProtocolWidgets();
	void activateProtocolWidgets();
	void startLogRead();
	void search();
	LogEntryList* searchEntries(LogEntryList* list);
	void insertChilds(LogEntryList* entries, QTreeWidgetItem* item, int index);
	LogEntryList* filterEntries(QString key, int error, int info, int update, int settings, LogEntryList * list);
	void deleteItem(int index);
	
	void insertSelectedItem(QTreeWidgetItem* item, int column);
	
	
	//test
	 void KeyPress();
	//test
	
	

signals:
	void logReadStop(bool free);
	
public:
    LogView(QWidget* parent);

    ~LogView();

};

#endif
