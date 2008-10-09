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
#include "juliuscontrol.h"

/**
 * @brief Constructor
 * 
 * 	Creates a new QProcess and connects the signals/slots of it
 *
 *	@author Peter Grasch
*/
JuliusControl::JuliusControl()
{
	julian = new QProcess();
	connect ( julian, SIGNAL(readyReadStandardOutput()), this, SLOT(processOutput()));
	connect ( julian, SIGNAL(readyReadStandardError()), this, SLOT(processError()));

	connect ( julian, SIGNAL(finished(int, QProcess::ExitStatus)), this, SIGNAL(juliusStopped(int)) );
}


/**
 * @brief Starts the Julius process
 * 
 * 	Starts the process by using 
 *
 *	@author Peter Grasch
 *	@param QString command
 *	The command to call
 *	@param QString paramter
 *	Parameter to the execution
 */
void JuliusControl::startJulius(QString command, QString parameter)
{
	julian->start(command + " " + parameter);
}

/**
 * @brief Stops the process
 * 
 * 	If the process is Running it is stopped
 *
 *	@author Peter Grasch
 */
void JuliusControl::stopJulius()
{
	if (julian->state() != QProcess::NotRunning)
		julian->terminate();
}


/**
 * @brief Search for the recogniced string
 * 
 * 	Uses the encapsulating <s> </s> tags
 * 	Emits the wordRecognised Signal with the recognised word
 *
 *	@author Peter Grasch
 *	@todo implement sampa/samparaw
 */
void JuliusControl::processOutput()
{
	QByteArray out;
	out= julian->readAllStandardOutput();
	if (! out.contains("<s>")) return;
	out= out.simplified();
	out = out.mid(out.lastIndexOf("<s>") + 3);
	out = out.left (out.lastIndexOf("</s>"));
	emit wordRecognised(out.trimmed(), "", ""); 
}

/**
 * @brief Process the Errormessage
 * 
 * 	Gets the Errormessages from stderr and emmits the signal
 * 	errorOccured(errormessage)
 *
 *	@author Peter Grasch
 */
void JuliusControl::processError()
{
	QByteArray out;
	out= julian->readAllStandardError();
// 	qDebug() << out;
	emit errorOccured(out);
}

/**
 * @brief Destructor
 * 
 *	@author Peter Grasch
 */
JuliusControl::~JuliusControl()
{
	julian->deleteLater();
}


