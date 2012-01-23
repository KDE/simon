/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#include "conditionconfiguration.h"

ConditionConfiguration::ConditionConfiguration(QWidget *parent,
const QByteArray &internalName, const KLocalizedString &name, const QByteArray &version,
const KLocalizedString &desc, const QString &iconName, const KComponentData &componentData,
const QVariantList &args) : QWidget(parent)
{
    Q_UNUSED(args);
    Q_UNUSED(componentData);

    m_about = new KAboutData(internalName, "", name,
                           version, desc, KAboutData::License_GPL);
    m_about->setProgramIconName(iconName);
}

ConditionConfiguration::~ConditionConfiguration()
{
  delete m_about;
}

void ConditionConfiguration::slotChanged()
{
  emit changed(true);
}
