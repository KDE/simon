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

#ifndef SIMON_LOGVIEW_H_F5D6909574D848B8A8A4D41704B11FB9
#define SIMON_LOGVIEW_H_F5D6909574D848B8A8A4D41704B11FB9

#include "ui_logmanagerdlg.h"
#include "logmanager.h"
#include <QWidget>

/**
 * \class LogView
 * \brief To display and search the logs
 * \version 0.1
 * \date 23.8.2007
 * \author Peter Grasch
 */
class LogView : public QWidget
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
    bool init();

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
    void cleanAllChildren();
    void deleteItem(int index);
    void deleteItem(QTreeWidgetItem *item);
    void deleteChildren(QTreeWidgetItem *item);
    void deleteChildren(int parentIndex);
    LogEntryList* filterFor(LogEntryList* log, bool copy,
      const QString filter, const short type);

    signals:
    void stopLogRead();

  public:
    LogView(QWidget* parent);
    ~LogView();

};
#endif
