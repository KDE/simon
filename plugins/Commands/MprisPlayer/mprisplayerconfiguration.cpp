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

K_PLUGIN_FACTORY_DECLARATION(MprisPlayerPluginFactory)

MprisPlayerConfiguration::MprisPlayerConfiguration(Scenario *parent)
    : CommandConfiguration(parent, "mprisplayer", ki18n("Media Player Control"),
                           "0.1", ki18n("Control MPRIS enabled media players"),
                           "applications-multimedia", MprisPlayerPluginFactory::componentData())
{
    ui.setupUi(this);

    QObject::connect(ui.playerNameLineEdit, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
}

MprisPlayerConfiguration::~MprisPlayerConfiguration()
{
}

QString MprisPlayerConfiguration::mediaPlayerServiceName()
{
    return ui.playerNameLineEdit->text();
}

void MprisPlayerConfiguration::setMediaPlayerServiceName(const QString& serviceName)
{
    ui.playerNameLineEdit->setText(serviceName);
}

bool MprisPlayerConfiguration::deSerialize(const QDomElement& elem)
{
    QDomElement serviceNameElem = elem.firstChildElement("serviceName");
    setMediaPlayerServiceName(serviceNameElem.text());

    return true;
}

QDomElement MprisPlayerConfiguration::serialize(QDomDocument *doc)
{
    QDomElement configElem = doc->createElement("config");
    QDomElement serviceNameElem = doc->createElement("serviceName");
    serviceNameElem.appendChild(doc->createTextNode(mediaPlayerServiceName()));
    configElem.appendChild(serviceNameElem);

    return configElem;
}

void MprisPlayerConfiguration::defaults()
{
    setMediaPlayerServiceName("org.mpris.MediaPlayer2.plasma-mediacenter");
}


