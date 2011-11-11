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

#ifndef CONDITIONCONFIGURATION_H
#define CONDITIONCONFIGURATION_H

/** \file conditionconfiguration.h
 * \brief The file containing the ConditionConfiguration baseclass header.
 */

#include <QWidget>
#include "simoncontextdetection_export.h"

class KComponentData;
class KAboutData;

class SIMONCONTEXTDETECTION_EXPORT ConditionConfiguration : public QWidget
{
    Q_OBJECT

protected:
    KAboutData *m_about;

signals:
    void changed(bool);

private slots:
    void slotChanged();

public:
    ConditionConfiguration(QWidget *parent, const QByteArray& internalName, const KLocalizedString& name,
                         const QByteArray& version, const KLocalizedString& desc,
                         const QString& iconName, const KComponentData& componentData,
                         const QVariantList &args = QVariantList());

    ~ConditionConfiguration();

    KAboutData* aboutData() { return m_about; }

public slots:
    virtual bool deSerialize(const QDomElement& element)=0;
    virtual QDomElement serialize(QDomDocument *doc)=0;
    virtual void defaults()=0;
};

#endif // CONDITIONCONFIGURATION_H
