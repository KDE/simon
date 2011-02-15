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
        m_dbusinterface( NULL ),
        m_isconnected(false),
        m_timer( this )
{
    setBackgroundHints ( DefaultBackground );
    setHasConfigurationInterface ( true );
    resize ( 200, 100 );
}

simonoid::~simonoid()
{
    delete m_dbusinterface;
}

void simonoid::init()
{   
    if (m_icon.isNull()) {
        setFailedToLaunch(true, i18n("Could not load simon icon!"));
	exit(-1);
    }
    
    connect( &m_timer, SIGNAL(timeout()), this, SLOT(checkConnection()) );
    m_timer.start(1000*CHECK_CONNECTION_INTERVAL_S);
    checkConnection();
}

void simonoid::checkConnection() {
  if(!m_isconnected) {    
    if(!connectSignalsAndSlots()) { 
      kDebug() << "waiting for simon to start...";
    } 
    else {
      kDebug() << "connected successful!";
      m_status = i18n("Waiting");
      m_peak = 0;
      update();
    }
  } else {
    if(!m_dbusinterface->isValid()) {
      kDebug() << "connection lost!";
      disconnectSignalsAndSlots();
      update();
    } else {
      kDebug() << "connection still valid";
    }
  }
}

bool simonoid::connectSignalsAndSlots() 
{    
    if( m_dbusinterface == NULL ) 
      m_dbusinterface = new QDBusInterface ( "org.kde.simon",
                                           "/SimonSender",
                                           "local.SimonSender" );
      
    if(m_dbusinterface == NULL) return false;
    
    bool success = true;
    if(success) {
      success = connect ( m_dbusinterface, SIGNAL ( listening() ), this, SLOT ( listeningCalled() ) );
      kDebug() << "connecting listening:" << (success?"connected":"disconnected") ;
    }    
    if(success) {
      success = connect ( m_dbusinterface, SIGNAL ( processing() ), this, SLOT ( processingCalled() ) );
      kDebug() << "connecting processing:" << (success?"connected":"disconnected") ;
    }
    if(success) { 
      success = connect ( m_dbusinterface, SIGNAL ( receivedResults() ), this, SLOT ( receivedResultsCalled() ) );
      kDebug() << "connecting receivedResults:" << (success?"connected":"disconnected") ;
    }
    if(success) {
      success = connect ( m_dbusinterface, SIGNAL ( recordingLevel ( double ) ), this, SLOT ( recordingLevelCalled ( double ) ) );
      kDebug() << "connecting recordingLevel:" << (success?"connected":"disconnected") ;
    }
    if(success) {
      m_isconnected = true;
    }
    else {
      disconnectSignalsAndSlots();
    }
    
    return success;
}


void simonoid::disconnectSignalsAndSlots() {
    //if(m_dbusinterface != NULL) disconnect(m_dbusinterface,0,0,0); TODO
    if(m_dbusinterface) delete m_dbusinterface;
    m_dbusinterface = NULL;
    m_isconnected = false;
}

void simonoid::paintInterface ( QPainter *p,
                                 const QStyleOptionGraphicsItem *option, const QRect &contentsRect )
{
    //kDebug() << "paintInterface called:" << (m_isconnected?"connected":"disconnected") ;
    QString text;    
    if( !m_isconnected ) {
      text = i18n ( "Simon not running" );    
    } else {
      text = i18n ( "Status:  %1\nPeak:    %2\%" ).arg ( m_status ).arg ( qRound(m_peak*100) );
    }
    
    int winsize_x = ( int ) this->size().width();
    int winsize_y = ( int ) this->size().height();
    
    int img_size = ( winsize_x < winsize_y ? winsize_x : winsize_y )-40;
    
    int margin = (winsize_y-img_size)/2;
    
    p->drawPixmap ( winsize_x-img_size-margin, margin, m_icon.pixmap( img_size, img_size ));
    p->save();
    
    p->setPen ( Qt::black );    
    //p->setFont ( QFont ( "Arial", 15, QFont::Bold ) );
    p->drawText ( contentsRect,
                  Qt::AlignVCenter | Qt::AlignLeft,
                  text );
    p->restore();
}

void simonoid::listeningCalled()
{
    m_status = i18n("Listening");
    update();
}

void simonoid::processingCalled()
{
    m_status = i18n("Processing");
    update();
}

void simonoid::receivedResultsCalled()
{
    m_status = i18n("Waiting");
    update();
}

void simonoid::recordingLevelCalled ( double peak )
{
    m_peak = peak;
    update();
}

#include "simonoid.moc"
