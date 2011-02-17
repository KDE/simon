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

#include <QPainter>
#include <QFontMetrics>
#include <KConfigGroup>
#include <KLocale>
#include <QDBusInterface>
#include <KConfigDialog> 

#include "simonoid.h"

simonoid::simonoid ( QObject *parent, const QVariantList &args )
        : Plasma::Applet ( parent, args ),
        m_icon ( "simon" ),
        m_isconnected(false),
        m_dbusinterface( NULL ),
        m_timer( this )
{
    setHasConfigurationInterface(true);
    setBackgroundHints ( DefaultBackground );
    setHasConfigurationInterface ( true );
}

simonoid::~simonoid()
{
    delete m_dbusinterface;
}


void simonoid::saveState( KConfigGroup &group ) const
{
    Plasma::Applet::saveState(group);
    kDebug() << "FUUUUUUUUUUUUUUU: save called";
    QMap<QString,QString> map = group.entryMap();
    kDebug() << "Entries: " << map;
    
    QString key;
    foreach( key, map.keys() )
      qDebug() << key << " = " << map[key];
    
    kDebug() << QString() + "Access: " + ( group.accessMode() == KConfigGroup::ReadWrite ? "YES" : "NO" );
    kDebug() << "i m gonna writhe somthing over this: " + group.readEntry("foo","nothing here yet...");
    group.writeEntry("foo","barxxxxrrR!");
    kDebug() << "just wrote somthing: " + group.readEntry("foo","dont know???");
}

void simonoid::restore(KConfigGroup& group)
{
    Plasma::Applet::restore(group);
    kDebug() << "FUUUUUUUUUUUUUUU: restore called";
    
    QMap<QString,QString> map = group.entryMap();
    kDebug() << "Entries: " << map;
    
    QString key;
    foreach( key, map.keys() )
      qDebug() << key << " = " << map[key];
    
    
    kDebug() << QString() + "Access: " + ( group.accessMode() == KConfigGroup::ReadWrite ? "YES" : "NO" );
    kDebug() << "foo=" + group.readEntry("foo","dont know???");
    foo = group.readEntry("foo","dont know???");
}

void simonoid::init()
{
    Plasma::Applet::init();
    
    kDebug() << "FUUUUUUUUUUUUUUU: init called";
    QMap<QString,QString> map = globalConfig().entryMap();
    kDebug() << "Entries: " << map;
    
    kDebug() << "init foo=" + config().readEntry("foo","dont know???");
  
    if (m_icon.isNull()) {
        setFailedToLaunch(true, i18n("Could not load simon icon!"));
	exit(-1);
    }
    
    m_appletLayout = new QGraphicsGridLayout();
    m_meter = new Plasma::Meter;
    m_meter->setVisible(true);
    m_meter->setMeterType(Plasma::Meter::BarMeterVertical);
    m_meter->setMaximumWidth(32);
    m_meter->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Expanding);
    m_meter->setMaximum(100);
    m_meter->setValue(66);
    
    m_lb_status = new Plasma::Label;
    m_lb_status->setText(i18n("Status:"));
    m_lb_status_value = new Plasma::Label;
    m_lb_status_value->setText("1");
    
    m_lb_peak = new Plasma::Label;
    m_lb_peak->setText(i18n("Peak:"));
    m_lb_peak_value = new Plasma::Label;
    m_lb_peak_value->setText("2");    
    
    m_simonicon = new Plasma::IconWidget();
    m_simonicon->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Maximum);
    m_simonicon->setIcon(m_icon);
    m_simonicon->setOrientation(Qt::Vertical);
    m_simonicon->setDrawBackground(true);
    //m_simonicon->setMaximumHeight(36);
    m_simonicon->setAcceptDrops(false);
    
    m_appletLayout->addItem( m_meter, 0, 0, 20, 10 );
    m_appletLayout->addItem( m_lb_status, 0, 10 );
    m_appletLayout->addItem( m_lb_status_value, 0, 20 );
    m_appletLayout->addItem( m_lb_peak, 10, 10 );
    m_appletLayout->addItem( m_lb_peak_value, 10, 20 );
    m_appletLayout->addItem( m_simonicon, 0, 30, 20, 10 );
    this->setLayout(m_appletLayout);
   
    initLayout(LAYOUT_LARGE);
    m_interval = 3; // TODO delete me    
    
    connect( &m_timer, SIGNAL(timeout()), this, SLOT(checkConnection()) );
    m_timer.start(1000*m_interval);
        
    checkConnection();
    update();
}

void simonoid::checkConnection() {
    emit configNeedsSaving();
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
    switch( m_layouttype )
    {
      case LAYOUT_TINY:
	break;
      case LAYOUT_SMALL:
	if( !m_isconnected ) {
	  text = i18n ( "Simon not running" );
	  text += QString("\nDEBUG: foo=%3").arg(foo); 
	} else {
	  text = i18n ( "Peak:    %2\%" ).arg ( m_status ).arg ( qRound(m_peak*100) );
	}
	break;
      case LAYOUT_LARGE:
      case LAYOUT_OVERKILL:
	if( !m_isconnected ) {
	  m_lb_status_value->setText ( i18n( "Simon not running") );
	} else {
	  m_lb_status_value->setText ( m_status );
	  m_lb_peak_value->setText ( QString() + qRound(m_peak*100) + "%" );
	}
	break;
    }
    
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

void simonoid::createConfigurationInterface( KConfigDialog* parent )
{
    QWidget *widget = new QWidget();
    m_uiconfig.setupUi(widget);
    m_uiconfig.ni_interval->setValue( m_interval );
    m_uiconfig.cb_layout->setCurrentIndex( m_layouttype );
    
    parent->setButtons( KDialog::Ok | KDialog::Cancel | KDialog::Apply );
    connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
    connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
    parent->addPage(widget, parent->windowTitle(), "chronometer");
    
    connect( parent, SIGNAL( widgetModified() ), this, SLOT(recordingLevelCalled()) );
}

void simonoid::configAccepted()
{
    m_interval = m_uiconfig.ni_interval->value();
    m_timer.setInterval(1000*m_interval);
    
    initLayout( (LAYOUT_TYPE) m_uiconfig.cb_layout->currentIndex() );
    
    update();
}


void simonoid::initLayout(simonoid::LAYOUT_TYPE type)
{
    m_meter->setVisible(false);
    m_simonicon->setVisible(false);
    m_lb_peak->setVisible(false);
    m_lb_peak_value->setVisible(false);
    m_lb_status->setVisible(false);
    m_lb_status_value->setVisible(false);
    
    switch( type )
    {
      case LAYOUT_TINY:
	m_meter->setVisible(true);
	resize ( 50, 50 );
	break;
      case LAYOUT_SMALL:
	m_meter->setVisible(true);
	m_simonicon->setVisible(true);
	resize ( 100, 100 );
	break;
      case LAYOUT_LARGE:
	m_meter->setVisible(true);
	m_simonicon->setVisible(true);
	m_lb_peak->setVisible(true);
	m_lb_peak_value->setVisible(true);
	m_lb_status->setVisible(true);
	m_lb_status_value->setVisible(true);
	resize ( 230, 150 );
	break;
      case LAYOUT_OVERKILL:
	m_meter->setVisible(true);
	m_simonicon->setVisible(true);
	m_lb_peak->setVisible(true);
	m_lb_peak_value->setVisible(true);
	m_lb_status->setVisible(true);
	m_lb_status_value->setVisible(true);
	resize ( 400, 300 );
	break;
      default:
	exit(-1); // TODO: invalid layout handling
    }
    
    m_layouttype = type;
}


#include "simonoid.moc"
