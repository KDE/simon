/*
 *  Copyright (C) 2009 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "accuracydisplay.h"
#include <math.h>
#include <QProgressBar>
#include <QLabel>
#include <QHBoxLayout>
#include <KLocalizedString>

AccuracyDisplay::AccuracyDisplay(const QString& name, int count, int correct, float accuracy, QWidget* parent) : QWidget(parent),
lbName(new QLabel(this)),
lbBreakdown(new QLabel(this)),
pbAccuracy(new QProgressBar(this))
{
  QHBoxLayout *lay = new QHBoxLayout(this);

  lbName->setText(name);

  lbBreakdown->setText(i18nc("%1 is the amount of correctly recognized samples, %2 is the amount of total samples", 
                             "Recognized %1 of %2: Recognition rate:", correct, count));

  pbAccuracy->setMaximumWidth(200);
  pbAccuracy->setMaximum(100);
  pbAccuracy->setValue(round(accuracy*100.0f));

  QSpacerItem *spacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);
  //QSpacerItem *spacer2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

  lay->addWidget(lbName);
  lay->addItem(spacer);
  lay->addWidget(lbBreakdown);
  lay->addWidget(pbAccuracy);
  show();

}


AccuracyDisplay::~AccuracyDisplay()
{
  lbName->deleteLater();
  lbBreakdown->deleteLater();
  pbAccuracy->deleteLater();
}
