/***************************************************************************
 *   Copyright (C) %{CURRENT_YEAR} by %{AUTHOR} <%{EMAIL}>                            *
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
 *   51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA .        *
 ***************************************************************************/

// Here we avoid loading the header multiple times
#ifndef SIMONOID2_HEADER
#define SIMONOID2_HEADER

// We need the Plasma Applet headers
#include <Plasma/Applet>
//#include <Plasma/Svg>

#include <KIcon>
#include <QString>
//#include <QTimer>

//class QSizeF;
class QDBusInterface;

// Define our plasma Applet
class simonoid2 : public Plasma::Applet
{
    Q_OBJECT
    public:
        // Basic Create/Destroy
        simonoid2(QObject *parent, const QVariantList &args);
        ~simonoid2();

        // The paintInterface procedure paints the applet to the desktop
        void paintInterface(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                const QRect& contentsRect);
        void init();

    private:
        //Plasma::Svg m_svg;
        KIcon m_icon;
	QString m_status;
	//QTimer m_timer;
	//static const int REFRESH_INTERVAL_MS = 500;
	QDBusInterface* m_dbusinterface;
	//int m_counter;
	double m_peak;
	
    private slots:
	void listeningCalled();
	void processingCalled();
	void receivedResultsCalled();
	void recordingLevelCalled( double peak );
};
 
// This is the command that links your applet to the .desktop file
K_EXPORT_PLASMA_APPLET(simonoid2, simonoid2)
#endif
