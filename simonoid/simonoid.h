/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2010 Manfred Scheucher <deadalps@gmail.com>
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

#ifndef SIMONOID2_HEADER
#define SIMONOID2_HEADER

#include <Plasma/Applet>
#include <KIcon>
#include <QString>
#include <QTimer>

class QDBusInterface;

class simonoid : public Plasma::Applet
{
    Q_OBJECT
    public:
        simonoid(QObject *parent, const QVariantList &args);
        ~simonoid();

        void paintInterface(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                const QRect& contentsRect);
        void init();

    private:
	bool connectSignalsAndSlots();
	void disconnectSignalsAndSlots();
	bool m_isconnected;
	
        KIcon m_icon;
	QString m_status;
	QDBusInterface* m_dbusinterface;
	double m_peak;
	QTimer m_timer;
	const static int CHECK_CONNECTION_INTERVAL_S = 3;
	
    private slots:
	void checkConnection();
	void listeningCalled();
	void processingCalled();
	void receivedResultsCalled();
	void recordingLevelCalled( double peak );
	
};
 
// linking the applet to the .desktop file
K_EXPORT_PLASMA_APPLET(simonoid, simonoid)
#endif
