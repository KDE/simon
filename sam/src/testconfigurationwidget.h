/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *  Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#ifndef SIMON_TESTCONFIGURATIONWIDGET_H_4002119636CC42C68FE07273F9000A73
#define SIMON_TESTCONFIGURATIONWIDGET_H_4002119636CC42C68FE07273F9000A73

#include "corpusinformation.h"
#include <QFrame>
#include <QDomElement>
#include <QUrl>

class QDomDocument;

class TestConfigurationWidget : public QFrame
{
  Q_OBJECT

  signals:
    void tagChanged();
    void changed();

  private:

  private slots:
    void remove();
    void updateTag(const QString&);

  protected:
    CorpusInformation* m_corpusInfo;
    int m_sampRate;
    QUrl m_testPrompts;
    QUrl m_testPromptsBasePath;

    virtual void setupUi() = 0;
    void updateGeneralParams(const QUrl& testPromptsUrl,
                             const QUrl& testPromptsBasePathUrl, int sampleRate);

  public slots:
//    void retrieveTag();

  public:
    enum BackendType {
      SPHINX=0,
      JHTK=1
    };

    TestConfigurationWidget(QWidget *parent=0);
    TestConfigurationWidget(CorpusInformation* corpusInfo,
        const QUrl& testPromptsUrl, const QUrl& testPromptsBasePathUrl,
        int sampleRate, QWidget *parent=0);

    static BackendType intToBackendType(int type);
    static BackendType stringToBackendType(const QString &typr);

    static TestConfigurationWidget *deSerialize(const QDomElement& elem);

    virtual ~TestConfigurationWidget();

    virtual BackendType getBackendType() const = 0;
    virtual void init(const QHash<QString, QString>&) = 0;

    QString tag() const { return m_corpusInfo->tag(); }
    CorpusInformation* corpusInformation() { return m_corpusInfo; }
    QUrl testPrompts() const { return m_testPrompts; }
    QUrl testPromptsBasePath() const { return m_testPromptsBasePath; }

    int sampleRate() const { return m_sampRate; }

    virtual QDomElement serialize(QDomDocument *doc);


};
#endif
