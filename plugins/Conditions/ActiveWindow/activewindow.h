/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef ACTIVEWINDOW_H
#define ACTIVEWINDOW_H

/** \file activewindow.h
 * \brief The file containing the ActiveWindow baseclass header.
 */

#include "simoncontextdetection/condition.h"
#include "simoncontextdetection/processinfo.h"
#include "simoncontextdetection/simoncontextdetection_export.h"
#include "simoncontextdetection/compoundcondition.h"
#include <QRegExp>

/**
 *	@class ActiveWindow
 *	@brief The ActiveWindow class is a condition plugin that monitors the active window name and its process name
 *
 *      The ActiveWindow will be satisfied whenever the process name and window name that it is monitoring (\ref m_processName \ref m_windowName )
 *      are the process name and window name of the active window as determined by the ProcessInfo singleton.  Upon initialization, the
 *      active window is checked, and afterwards, the changes to the active window are checked by connecting the
 *      ProcessInfo signals \ref ProcessInfo::activeWindowTitleChanged(QString) and \ref ProcessInfo::activeWindowProcessChanged(QString) to the
 *      ActiveWindow slots \ref checkActiveWindowTitle(QString) and \ref checkActiveWindowProcess(QString) respectively.
 *
 *      Just like any other condition, the ActiveWindow will accordingly update its \ref Condition::m_satisfied value and then emit its \ref Condition::conditionChanged()
 *      signal whenever it becomes newly satisfied or unsatisfied.
 *
 *      \sa Condition, ProcessInfo, CreateActiveWindowWidget
 *
 *	@version 0.1
 *	@date 8.3.2011
 *	@author Adam Nash
 */

class ActiveWindow : public Condition
{
    Q_OBJECT
public:
    explicit ActiveWindow(QObject *parent, const QVariantList& args);

    QString getProcessName() {return m_processName;}
    QString getWindowName() {return m_windowName;}
    bool getWindowNameIsRegExp() {return m_windowNameIsRegularExpression;}

    virtual QString name();

    virtual CreateConditionWidget* getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget *parent);

private:
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);
    void checkBothNames();

    QString m_processName;
    QString m_windowName;

    bool m_windowNameIsRegularExpression;
    QRegExp m_windowNameRegExp;

    QString m_currentProcessName;
    QString m_currentWindowName;

signals:

public slots:
    void checkActiveWindowTitle(QString title);
    void checkActiveWindowProcess(QString processName);
};

#endif // ACTIVEWINDOW_H
