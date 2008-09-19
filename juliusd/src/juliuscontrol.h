/***************************************************************************
 *   Copyright (C) 2006 by Peter Grasch   *
 *   bedahr@gmx.net   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
#ifndef JULIUSCONTROL_H
#define JULIUSCONTROL_H

#include <QObject>
#include <QProcess>

/**
 *	@class JuliusControl
 *	@brief Controls the juli(-us/-an) instance
 *
 *	Fetches the output of the juli(-us/-an) process and parses it to
 *	get the recognition results.
 *	Once it has detected that a word has been recognised, it emits the
 *	wordRecognised(QString) signal
 *	It also provides an errorOccured() signal for the stderr resp.
 *
 *	@version 0.1
 *	@date 11.02.2007
 *	@author Peter Grasch
 */
class JuliusControl : public QObject{
	Q_OBJECT
private:
	QProcess *julian; //!< julian Control
signals:
	void wordRecognised(QString,QString,QString);
	void errorOccured(QString);
	void juliusStopped(int);
public slots:
	void processOutput();
	void processError();
	void stopJulius();
	void startJulius(QString command="julian", QString parameter="-input mic -C julian.jconf -quiet");
public:
    JuliusControl();

    ~JuliusControl();

};

#endif
