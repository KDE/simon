/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_TRAINSAMPLEPAGE_H_3CCB79FEB912407BB45C275731A2C306
#define SIMON_TRAINSAMPLEPAGE_H_3CCB79FEB912407BB45C275731A2C306

#include <QString>
#include <QWizardPage>
class RecWidget;
class QSettings;
class DeviceInformationPage;

class TrainSamplePage : public QWizardPage
{
  Q_OBJECT

    private:
    QString m_name;
    RecWidget *recorder;
	DeviceInformationPage *m_forcedDevices;
    QString prompt;
    QString fileName;
    int m_thisPage;
    int m_maxPage;
    QString m_directory;

  public:
    TrainSamplePage(const QString& name, QString prompt, int nowPage, int maxPage,
      const QString& directory, QWidget *parent=0,
      const QString& forcedFileNameTemplate=QString(),
	  DeviceInformationPage *forcedDevices=0);
    ~TrainSamplePage();
    bool isComplete() const;

    void initializePage();
    bool validatePage();
    void cleanupPage();

    void setupUi();

    QString getPrompt() const { return prompt; }
    QStringList getFileNames() const;

    QStringList getDevices();

    bool serializeToStorage(QSettings& ini, const QString& directory) const;
    bool deserializeFromStorage(QSettings& ini, const QString& directory);

  public slots:
    bool submit();
    bool cleanUp();

};
#endif
