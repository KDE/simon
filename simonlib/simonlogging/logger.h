/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_LOGGER_H_DA7C3A7BA63E42D7A49216FBDDA84C6D
#define SIMON_LOGGER_H_DA7C3A7BA63E42D7A49216FBDDA84C6D

#include "simonlogging_export.h"
#include <QString>
/**
 \class Logger
 \author Peter Grasch
 \brief Logs messages to the logfile with static functions
*/

class QTextStream;
class QFile;
class QMutex;

class SIMONLOGGING_EXPORT Logger
{
  private:
    static QTextStream *logFile;
    static QFile *logF;
    static QMutex *lock;

  public:
    enum LogType {
      Info=1,
      Warning=2,
      Error=4
    };
    /**
     * \brief Initialises the file handle and the stream
     * \author Peter Grasch
     * @param path
     * The path to the file - will be created if it doesn't exist
     * @return
     * success
     */
    static QTextStream* init();

    /**
     * \brief Logs the given string to the file (adds a timecode)
     * \author Peter Grasch
     * @param message
     * The message to log
     */
    static void log(QString message, LogType type=Info);

    /**
     *        \brief Closes and flushes the buffer
     *        \author Peter Grasch
     */
    static void close();

};
#endif
