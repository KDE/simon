/*
 *   Copyright (C) 2014 Ashish Madeti <ashishmadeti@gmail.com>
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

#include "mprisplayerconfiguration.h"
#include "mprisconstants.h"

#include <QDBusConnection>
#include <QDBusConnectionInterface>
#include <QDBusReply>

K_PLUGIN_FACTORY_DECLARATION(MprisPlayerPluginFactory)

MprisPlayerConfiguration::MprisPlayerConfiguration(Scenario *parent)
    : CommandConfiguration(parent, "mprisplayer", i18n("Media Player Control"),
                           "0.1", i18n("Control MPRIS enabled media players"),
                           "applications-multimedia")
{
    ui.setupUi(this);

    QObject::connect(ui.rbSupportAll, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
    QObject::connect(ui.rbSupportOne, SIGNAL(toggled(bool)), this, SLOT(slotChanged()));
    QObject::connect(ui.cbMediaServiceNames, SIGNAL(currentIndexChanged(int)), this, SLOT(slotChanged()));
    QObject::connect(ui.rbSupportOne, SIGNAL(clicked()), this, SLOT(populateMediaServices()));
}

MprisPlayerConfiguration::~MprisPlayerConfiguration()
{
}

bool MprisPlayerConfiguration::deSerialize(const QDomElement& elem)
{
    QDomElement supportAllElem = elem.firstChildElement("supportAll");
    if (supportAllElem.text() == "") {
        setSupportAll(true);
    } else {
        setSupportAll(static_cast<bool>(supportAllElem.text().toInt()));
    }

    QDomElement serviceNameElem = elem.firstChildElement("serviceName");
    QString selectedMediaService = serviceNameElem.text();
    if (!selectedMediaService.isEmpty()) {
        int selectedPos = ui.cbMediaServiceNames->findText(selectedMediaService);
        if (selectedPos != -1) {
            ui.cbMediaServiceNames->setCurrentIndex(selectedPos);
        } else {
            ui.cbMediaServiceNames->insertItem(0, selectedMediaService);
            ui.cbMediaServiceNames->setCurrentIndex(0);
        }
    }

    return true;
}

QDomElement MprisPlayerConfiguration::serialize(QDomDocument *doc)
{
    QDomElement configElem = doc->createElement("config");
    QDomElement supportAllElem = doc->createElement("supportAll");
    supportAllElem.appendChild(doc->createTextNode(QString::number(static_cast<int>(supportAll()))));
    configElem.appendChild(supportAllElem);

    QDomElement serviceNameElem = doc->createElement("serviceName");
    serviceNameElem.appendChild(doc->createTextNode(ui.cbMediaServiceNames->currentText()));
    configElem.appendChild(serviceNameElem);

    return configElem;
}

void MprisPlayerConfiguration::defaults()
{
    setSupportAll(true);
}

void MprisPlayerConfiguration::populateMediaServices()
{
    QDBusReply<QStringList> reply = QDBusConnection::sessionBus().interface()->registeredServiceNames();
    QStringList services = reply.value();
    foreach (const QString& serviceName, services) {
        if (serviceName.startsWith(MprisPlayerPrefix)
               && ui.cbMediaServiceNames->findText(serviceName) == -1) {
            ui.cbMediaServiceNames->addItem(serviceName);
        }
    }
}

bool MprisPlayerConfiguration::supportAll()
{
    return ui.rbSupportAll->isChecked();
}

QString MprisPlayerConfiguration::selectedMediaService()
{
    return ui.cbMediaServiceNames->currentText();
}

void MprisPlayerConfiguration::setSupportAll(bool b)
{
    if (b) {
        ui.rbSupportAll->click();
    } else {
        ui.rbSupportOne->click();
    }
}
