/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_DEVICEINFORMATIONPAGE_H_417017FF7135400DAC9F8BA0263C0FA5
#define SIMON_DEVICEINFORMATIONPAGE_H_417017FF7135400DAC9F8BA0263C0FA5

#include <QWizardPage>
#include <QList>
#include <QHash>

class QScrollArea;
class DeviceInformationWidget;
class Microphone;
class SoundCard;
class QSettings;

class DeviceInformationPage : public QWizardPage
{

  private:
    QList<DeviceInformationWidget*> informationWidgets;
    QScrollArea *scrollWidget;

    void registerInformationWidget(DeviceInformationWidget*);

  public:
    DeviceInformationPage(QWidget *parent=0);
    void initializePage();
    ~DeviceInformationPage();

    QStringList getDeviceStrings();

    bool serializeToStorage(QSettings& ini) const;
    bool deserializeFromStorage(QSettings& ini);

    QHash<QString, Microphone*> buildMicrophoneMappings(bool &ok);
    QHash<QString, SoundCard*> buildSoundCardMappings(bool &ok);

    bool isComplete() const;
    bool validatePage();

};
#endif
