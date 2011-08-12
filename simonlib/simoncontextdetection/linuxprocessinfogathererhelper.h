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


#ifndef LINUXPROCESSINFOGATHERERHELPER_H
#define LINUXPROCESSINFOGATHERERHELPER_H

#include <QObject>

class LinuxProcessInfoGathererHelper : public QObject
{
    Q_OBJECT
public:
    explicit LinuxProcessInfoGathererHelper(QObject *parent = 0);

private:
    /** \brief The current active window name/title
      *
      */
    QString m_currentActiveWindowTitle;
    /** \brief The current active window process
      *
      */
    QString m_currentActiveWindowProgram;

signals:
    /** \brief Emits the name of the active window name/title when it changes
      *
      */
    void activeWindowTitleChanged(QString);
    /** \brief Emits the name of the active window process when it changes
      *
      */
    void activeWindowProcessChanged(QString);

public slots:
    /** \brief Checks the current active window to see if it has changed
      *
      */
    void checkActiveWindow();

};

#endif // LINUXPROCESSINFOGATHERERHELPER_H
