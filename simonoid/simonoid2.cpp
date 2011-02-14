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

#include "simonoid2.h"
#include <QPainter>
#include <QFontMetrics>
#include <QSizeF>
#include <KLocale>
#include <QDBusInterface>

//#include <plasma/svg.h>
//#include <plasma/theme.h>

simonoid2::simonoid2 ( QObject *parent, const QVariantList &args )
        : Plasma::Applet ( parent, args ),
        //m_svg ( this ),
        m_icon ( "simon" ),
        //m_timer( this ),
        m_dbusinterface( NULL )
{
    setBackgroundHints ( DefaultBackground );
    setHasConfigurationInterface ( true );
    resize ( 280, 120 );
}

simonoid2::~simonoid2()
{
    if ( hasFailedToLaunch() )
    {
        // Do some cleanup here
    }
    else
    {
        // Save settings
    }
    delete m_dbusinterface;
}

void simonoid2::init()
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

    //connect ( &m_timer, SIGNAL ( timeout() ), this, SLOT ( update() ) );
    //m_timer.start( REFRESH_INTERVAL_MS );

    // A small demonstration of the setFailedToLaunch function
    if (m_icon.isNull()) {
        setFailedToLaunch(true, i18n("could not load simon icon"));
    }
}


void simonoid2::paintInterface ( QPainter *p,
                                 const QStyleOptionGraphicsItem *option, const QRect &contentsRect )
{
    //p->setRenderHint ( QPainter::SmoothPixmapTransform );
    //p->setRenderHint ( QPainter::Antialiasing );

    // Now we draw the applet, starting with our svg
    //m_svg.resize ( ( int ) contentsRect.width(), ( int ) contentsRect.height() );
    //m_svg.paint ( p, ( int ) contentsRect.left(), ( int ) contentsRect.top() );

    // We place the icon and text on our background
    //p->drawPixmap ( 25, 40, m_icon.pixmap ( ( int ) contentsRect.width(), ( int ) contentsRect.width()-50 ) );
    p->drawPixmap ( 180, 20, m_icon.pixmap ( 80,80 ));
    p->save();
    p->setPen ( Qt::black );
    p->setFont ( QFont ( "Arial", 15, QFont::Bold ) );
    p->drawText ( contentsRect,
                  Qt::AlignVCenter | Qt::AlignLeft,
                  i18n ( "status:  %1\npeak:    %2\%" ).arg ( m_status ).arg ( qRound(m_peak*100) ) );
    p->restore();
}


void simonoid2::listeningCalled()
{
    m_status = i18n("listening");
    update();
}


void simonoid2::processingCalled()
{
    m_status = i18n("processing");
    update();
}


void simonoid2::receivedResultsCalled()
{
    m_status = i18n("waiting");
    update();
}


void simonoid2::recordingLevelCalled ( double peak )
{
    //m_status = "recordingLevelCalled";
    m_peak = peak;
    update();
}


#include "simonoid2.moc"
