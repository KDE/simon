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
#include <QSizeF>
#include "ui_config.h"

class QDBusInterface;

class Simonoid : public Plasma::Applet {
  Q_OBJECT
public:

  Simonoid ( QObject *parent, const QVariantList &args );
  ~Simonoid();

  virtual void paintInterface ( QPainter *painter,
                                const QStyleOptionGraphicsItem *option,
                                const QRect& contentsRect );
  virtual void init();
  virtual void saveState ( KConfigGroup &group ) const;
  virtual void createConfigurationInterface ( KConfigDialog *parent );

protected:
  QSizeF sizeHint(Qt::SizeHint which, const QSizeF& constraint=QSizeF()) const;

private:
  enum LayoutType {
    LayoutInvalid=-1,
    LayoutTiny=0,
    LayoutSmall=1,
    LayoutLarge=2,
  };

  // GUI
  QGraphicsGridLayout *m_appletLayout;
  Plasma::Meter* m_meter;
  Plasma::Label* m_lb_status;
  Plasma::Label* m_lb_peak;
  Plasma::Label* m_lb_status_value;
  Plasma::Label* m_lb_peak_value;
  Plasma::IconWidget *m_simonicon;

  LayoutType m_layouttype;
  int m_interval;

  QString m_status;
  KIcon m_icon;
  bool m_isconnected;
  QWidget* m_configpage;
  Ui_Config m_uiconfig;

  QDBusInterface* m_dbusinterface;
  double m_peak;
  QTimer m_checkConnectionTimer;

  void initLayout ( LayoutType type );
  bool connectSignalsAndSlots();
  void disconnectSignalsAndSlots();

private slots:
  void checkConnection();
  void listeningCalled();
  void processingCalled();
  void receivedResultsCalled();
  void recordingLevelCalled ( double peak );
  void configAccepted();

};

// linking the applet to the .desktop file
K_EXPORT_PLASMA_APPLET ( simonoid, Simonoid )
#endif
