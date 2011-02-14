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

#include "simonoid.h"
#include <QPainter>
#include <QFontMetrics>
#include <KLocale>
#include <QDBusInterface>

simonoid::simonoid ( QObject *parent, const QVariantList &args )
        : Plasma::Applet ( parent, args ),
        m_icon ( "simon" ),
        m_dbusinterface( NULL )
{
    setBackgroundHints ( DefaultBackground );
    setHasConfigurationInterface ( true );
    resize ( 280, 120 );
}

simonoid::~simonoid()
{
    delete m_dbusinterface;
}

void simonoid::init()
{
    m_status = i18n("waiting");
    m_peak = 0;
    m_dbusinterface = new QDBusInterface ( "org.kde.simon",
                                           "/SimonSender",
                                           "local.SimonSender" );

    connect ( m_dbusinterface, SIGNAL ( listening() ), this, SLOT ( listeningCalled() ) );
    connect ( m_dbusinterface, SIGNAL ( processing() ), this, SLOT ( processingCalled() ) );
    connect ( m_dbusinterface, SIGNAL ( receivedResults() ), this, SLOT ( receivedResultsCalled() ) );
    connect ( m_dbusinterface, SIGNAL ( recordingLevel ( double ) ), this, SLOT ( recordingLevelCalled ( double ) ) );

    if (m_icon.isNull()) {
        setFailedToLaunch(true, i18n("could not load simon icon"));
    }
}

void simonoid::paintInterface ( QPainter *p,
                                 const QStyleOptionGraphicsItem *option, const QRect &contentsRect )
{
    p->drawPixmap ( 180, 20, m_icon.pixmap ( 80,80 ));
    p->save();
    p->setPen ( Qt::black );
    p->setFont ( QFont ( "Arial", 15, QFont::Bold ) );
    p->drawText ( contentsRect,
                  Qt::AlignVCenter | Qt::AlignLeft,
                  i18n ( "status:  %1\npeak:    %2\%" ).arg ( m_status ).arg ( qRound(m_peak*100) ) );
    p->restore();
}

void simonoid::listeningCalled()
{
    m_status = i18n("listening");
    update();
}

void simonoid::processingCalled()
{
    m_status = i18n("processing");
    update();
}

void simonoid::receivedResultsCalled()
{
    m_status = i18n("waiting");
    update();
}

void simonoid::recordingLevelCalled ( double peak )
{
    m_peak = peak;
    update();
}

#include "simonoid.moc"
