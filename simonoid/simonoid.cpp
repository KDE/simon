/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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
#include <KConfigGroup>
#include <KLocale>
#include <QDBusInterface>
#include <KConfigDialog>

Simonoid::Simonoid ( QObject *parent, const QVariantList &args )
  : Plasma::Applet ( parent, args ),
    m_appletLayout(0),
    m_layouttype ( Simonoid::LayoutInvalid ),
    m_interval ( -1 ),
    m_icon ( "simon" ),
    m_isconnected ( false ),
    m_configpage ( 0 ),
    m_dbusinterface ( 0 ),
    m_checkConnectionTimer ( this ) {
  setHasConfigurationInterface ( true );
  setBackgroundHints ( DefaultBackground );
  setHasConfigurationInterface ( true );
}

Simonoid::~Simonoid() {
  m_dbusinterface->deleteLater();
}

QSizeF Simonoid::sizeHint(Qt::SizeHint which, const QSizeF& constraint) const
{
  Q_UNUSED(constraint);
  
  if (which == Qt::PreferredSize) {
    switch (m_layouttype) {
      case LayoutTiny:
        return QSizeF(300, 50);
      case LayoutSmall:
        return QSizeF(300, 50);
      case LayoutLarge:
        return QSizeF(300, 220);
      default:
        return QSizeF(0,0);
    }
  }
  return Plasma::Applet::sizeHint(which, constraint);
}


void Simonoid::saveState ( KConfigGroup &group ) const {
  kDebug() << "Save state";
  Plasma::Applet::saveState ( group );
  
  KConfigGroup settings = config();
  settings.writeEntry ( "LayoutType", ( int ) m_layouttype );
  settings.writeEntry ( "RefreshInterval", m_interval );
  settings.sync();
}

void Simonoid::init() {
  kDebug() << "Restoring";
  KConfigGroup lconfig = config();
  m_layouttype = ( LayoutType ) lconfig.readEntry ( "LayoutType", ( int ) LayoutTiny);
  m_interval = lconfig.readEntry ( "RefreshInterval", 3 );

  kDebug() << "Restored to: " << m_layouttype << m_interval;
  
  kDebug() << "Init called";
  Plasma::Applet::init();

  if ( m_icon.isNull() ) {
    setFailedToLaunch ( true, i18n ( "Could not load Simon icon." ) );
    exit ( -1 );
  }

  m_meter = new Plasma::Meter;
  m_meter->setVisible ( true );
  m_meter->setMeterType ( Plasma::Meter::BarMeterHorizontal );
//   m_meter->setMaximumWidth ( 32 );
//   m_meter->setSizePolicy ( QSizePolicy::Maximum, QSizePolicy::Expanding );
  m_meter->setMaximum ( 100 );
  m_meter->setValue ( 0 );

  m_lb_status = new Plasma::Label;
  m_lb_status->setText ( i18n ( "Status:" ) );
  m_lb_status_value = new Plasma::Label;
  m_lb_status_value->setText ( "1" );

  m_lb_peak = new Plasma::Label;
  m_lb_peak->setText ( i18n ( "Peak:" ) );
  m_lb_peak_value = new Plasma::Label;
  m_lb_peak_value->setText ( "2" );

  m_simonicon = new Plasma::IconWidget();
//   m_simonicon->setSizePolicy ( QSizePolicy::Expanding, QSizePolicy::Maximum );
  m_simonicon->setIcon ( m_icon );
  m_simonicon->setOrientation ( Qt::Vertical );
  m_simonicon->setDrawBackground ( true );
  //m_simonicon->setMaximumHeight(36);
  m_simonicon->setAcceptDrops ( false );
  

  initLayout ( m_layouttype );
  m_interval = m_interval;

  connect ( &m_checkConnectionTimer, SIGNAL (timeout()), this, SLOT (checkConnection()) );
  m_checkConnectionTimer.start ( 1000*m_interval );

  checkConnection();
  update();
}

void Simonoid::checkConnection() {
  if ( !m_isconnected ) {
    if ( !connectSignalsAndSlots() ) {
      kDebug() << "waiting for Simon to start...";
    } else {
      kDebug() << "connected successful!";
      m_status = i18n ( "Waiting" );
      m_peak = 0;
      update();
    }
  } else {
    if ( !m_dbusinterface->isValid() ) {
      kDebug() << "connection lost!";
      disconnectSignalsAndSlots();
      update();
    } else {
      kDebug() << "connection still valid";
    }
  }
}

bool Simonoid::connectSignalsAndSlots() {
  if ( m_dbusinterface == NULL )
    m_dbusinterface = new QDBusInterface ( "org.kde.simon",
                                           "/SimonSender",
                                           "local.SimonSender" );

  if ( m_dbusinterface == NULL ) return false;

  bool success = true;
  if ( success ) {
    success = connect ( m_dbusinterface, SIGNAL (listening()), this, SLOT (listeningCalled()) );
    kDebug() << "connecting listening:" << ( success?"connected":"disconnected" ) ;
  }
  if ( success ) {
    success = connect ( m_dbusinterface, SIGNAL (processing()), this, SLOT (processingCalled()) );
    kDebug() << "connecting processing:" << ( success?"connected":"disconnected" ) ;
  }
  if ( success ) {
    success = connect ( m_dbusinterface, SIGNAL (receivedResults()), this, SLOT (receivedResultsCalled()) );
    kDebug() << "connecting receivedResults:" << ( success?"connected":"disconnected" ) ;
  }
  if ( success ) {
    success = connect ( m_dbusinterface, SIGNAL (recordingLevel(double)), this, SLOT (recordingLevelCalled(double)) );
    kDebug() << "connecting recordingLevel:" << ( success?"connected":"disconnected" ) ;
  }
  if ( success ) {
    m_isconnected = true;
  } else {
    disconnectSignalsAndSlots();
  }

  return success;
}


void Simonoid::disconnectSignalsAndSlots() {
  //if(m_dbusinterface != NULL) disconnect(m_dbusinterface,0,0,0); TODO
  if ( m_dbusinterface ) m_dbusinterface->deleteLater();
  m_dbusinterface = NULL;
  m_isconnected = false;
}


void Simonoid::paintInterface ( QPainter *p,
                                const QStyleOptionGraphicsItem *option, const QRect &contentsRect ) {
  Q_UNUSED ( p );
  Q_UNUSED ( option );
  Q_UNUSED ( contentsRect );

  m_meter->setValue(qRound(m_peak*100));
  QString text;
  switch ( m_layouttype ) {
  case LayoutTiny:
  case LayoutSmall:
    break;
//     if ( !m_isconnected ) {
//       text = i18n ( "Simon not running" );
//     } else {
//       text = i18n ( "Peak:    %2\%" ).arg ( m_status ).arg ( qRound ( m_peak*100 ) );
//     }
    break;
  case LayoutLarge:
    if ( !m_isconnected ) {
      m_lb_status_value->setText ( i18n ( "Simon not running" ) );
    } else {
      m_lb_status_value->setText ( m_status );
      m_lb_peak_value->setText ( QString ( "%1%" ).arg ( qRound ( m_peak*100 ) ) );
    }
    break;
  default:
    kWarning() << "Invalid layout";
    break;
  }

}

void Simonoid::listeningCalled() {
  m_status = i18n ( "Listening" );
  update();
}

void Simonoid::processingCalled() {
  m_status = i18n ( "Processing" );
  update();
}

void Simonoid::receivedResultsCalled() {
  m_status = i18n ( "Waiting" );
  update();
}

void Simonoid::recordingLevelCalled ( double peak ) {
  m_peak = peak;
  update();
}

void Simonoid::createConfigurationInterface ( KConfigDialog* parent ) {
  m_configpage = new QWidget();
  m_uiconfig.setupUi ( m_configpage );
  
  m_uiconfig.ni_interval->setValue ( m_interval );
  m_uiconfig.cb_layout->setCurrentIndex ( m_layouttype );

  parent->addPage ( m_configpage, parent->windowTitle(), "chronometer" );
  connect(parent, SIGNAL(applyClicked()), this, SLOT(configAccepted()));
  connect(parent, SIGNAL(okClicked()), this, SLOT(configAccepted()));
}

void Simonoid::configAccepted() {
  if ( m_configpage ) {
    m_layouttype = ( LayoutType ) m_uiconfig.cb_layout->currentIndex();
    m_interval = m_uiconfig.ni_interval->value();
  }

  m_checkConnectionTimer.setInterval ( 1000*m_interval );

  initLayout ( m_layouttype );

  update();
  emit configNeedsSaving();
  m_configpage->deleteLater();
}

void Simonoid::initLayout ( Simonoid::LayoutType type ) {
  if (type < 0) {
    kWarning() << "Invalid layout";
    return;
  }
  kDebug() << "Init";
  
  m_appletLayout = new QGraphicsGridLayout();
  for (int i=0; i < m_appletLayout->count(); i++)
   m_appletLayout->removeAt(i);
  
  m_meter->setVisible(false);
  m_lb_status->setVisible(false);
  m_lb_peak->setVisible(false);
  m_lb_status_value->setVisible(false);
  m_lb_peak_value->setVisible(false);
  m_simonicon->setVisible(false);
  
  switch (type) {
    case LayoutLarge:
      m_appletLayout->addItem ( m_lb_status, 0, 0 );
      m_appletLayout->addItem ( m_lb_status_value, 0, 1 );
      m_appletLayout->addItem ( m_lb_peak, 1, 0 );
      m_appletLayout->addItem ( m_lb_peak_value, 1, 1 );
      m_lb_status->setVisible(true);
      m_lb_peak->setVisible(true);
      m_lb_status_value->setVisible(true);
      m_lb_peak_value->setVisible(true);
    case LayoutSmall:
      m_appletLayout->addItem ( m_simonicon, 0, 2, 3, 1 );
      m_simonicon->setVisible(true);
    case LayoutTiny:
      m_appletLayout->addItem ( m_meter, 2, 0, 1, 2 );
      m_meter->setVisible(true);
      break;
    default:
      return;
  }
  
  this->setLayout ( m_appletLayout );
  
  m_layouttype = type;
  resize(sizeHint(Qt::PreferredSize));
  update();
}


#include "simonoid.moc"
