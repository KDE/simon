/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_TESTCONFIGURATIONwIDGET_H_4002119636CC42C68FE07273F9000A73
#define SIMON_TESTCONFIGURATIONwIDGET_H_4002119636CC42C68FE07273F9000A73

#include "ui_testconfiguration.h"
#include <QFrame>
#include <QDomElement>

class QDomDocument;

class CorpusInformation;

class TestConfigurationWidget : public QFrame
{
  Q_OBJECT

  signals:
    void tagChanged();
    void changed();

  private:
    CorpusInformation* m_corpusInfo;
    Ui::TestConfigurationWdg ui;
    void setupUi();
  private slots:
    void remove();
    void updateTag(const QString&);

  public slots:
    void retrieveTag();

  public:
    TestConfigurationWidget(QWidget *parent=0);
    TestConfigurationWidget(CorpusInformation* corpusInfo,
        const KUrl& hmmDefsUrl,
        const KUrl& tiedlistUrl, const KUrl& dictUrl, const KUrl& dfaUrl,
        const KUrl& testPromptsUrl, const KUrl& testPromptsBasePathUrl,
        const KUrl& jconfUrl, int sampleRate, QWidget *parent=0);

    ~TestConfigurationWidget();

    QString tag() const;
    CorpusInformation* corpusInformation() { return m_corpusInfo; }
    KUrl hmmDefs() const;
    KUrl tiedlist() const;
    KUrl dict() const;
    KUrl dfa() const;
    KUrl testPrompts() const;
    KUrl testPromptsBasePath() const;
    KUrl jconf() const;
    int sampleRate() const;
    
    static TestConfigurationWidget* deSerialize(const QDomElement& elem);
    QDomElement serialize(QDomDocument *doc);
};
#endif
