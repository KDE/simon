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
#include <Plasma/Meter>
#include <Plasma/Label>
#include <Plasma/IconWidget>
#include <QGraphicsGridLayout>
#include <KIcon>
#include <KConfigGroup>
#include <QString>
#include <QTimer>
#include <QWidget>
#include "ui_config.h"

class QDBusInterface;

class simonoid : public Plasma::Applet
{
    Q_OBJECT
    public:
	
        simonoid(QObject *parent, const QVariantList &args);
        ~simonoid();

        virtual void paintInterface(QPainter *painter,
                const QStyleOptionGraphicsItem *option,
                const QRect& contentsRect);
        virtual void init();
	virtual void saveState(KConfigGroup &group) const;
	virtual void restore( KConfigGroup &group );
	virtual void createConfigurationInterface(KConfigDialog *parent);

    private:
	// GUI
        QGraphicsGridLayout *m_appletLayout;
        Plasma::Meter* m_meter;
        Plasma::Label* m_lb_status;
        Plasma::Label* m_lb_peak;
        Plasma::Label* m_lb_status_value;
        Plasma::Label* m_lb_peak_value;
	
	enum LAYOUT_TYPE { LAYOUT_TINY=0, LAYOUT_SMALL=1, LAYOUT_LARGE=2, LAYOUT_OVERKILL=3 };
	int m_layouttype;
	bool connectSignalsAndSlots();
	void disconnectSignalsAndSlots();
	
	// stuff
	QString foo;
	QString m_status;
        KIcon m_icon;
	bool m_isconnected;
	QWidget *m_configpage;
	Ui_Config m_uiconfig;
	
	QDBusInterface* m_dbusinterface;
	double m_peak;
	QTimer m_timer;
	int m_interval;
	void initLayout(LAYOUT_TYPE type);
        //Plasma::Animation *m_ejectButtonAnimation;
        Plasma::IconWidget *m_simonicon;
	
    private slots:
	void checkConnection();
	void listeningCalled();
	void processingCalled();
	void receivedResultsCalled();
	void recordingLevelCalled( double peak );
	void configAccepted();
	
};
 
// linking the applet to the .desktop file
K_EXPORT_PLASMA_APPLET(simonoid, simonoid)
#endif
