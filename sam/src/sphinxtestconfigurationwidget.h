/*
 *   Copyright (C) 2012 Vladislav Sitalo <root@stvad.org>
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

#ifndef SPHINXTESTCONFIGURATIONWIDGET_H
#define SPHINXTESTCONFIGURATIONWIDGET_H

#include "testconfigurationwidget.h"
#include <QUrl>

namespace Ui {
class SphinxTestConfigurationWidget;
}

class SphinxTestConfigurationWidget : public TestConfigurationWidget
{
  Q_OBJECT
  
public:
  explicit SphinxTestConfigurationWidget(QWidget *parent = 0);
  SphinxTestConfigurationWidget(CorpusInformation* corpusInfo,
                               const QUrl& testPromptsUrl, const QUrl& testPromptsBasePathUrl,
                               int sampleRate, QWidget *parent=0);

  ~SphinxTestConfigurationWidget();

  QUrl sphinxModelDir () const;
  QUrl sphinxGrammar() const;
  QUrl sphinxDictionary() const;

  QDomElement serialize(QDomDocument *doc);

  void init(const QHash<QString, QString>&);
  BackendType getBackendType() const { return TestConfigurationWidget::SPHINX; }

protected:
  void setupUi();

protected slots:
   void update();

private:
  Ui::SphinxTestConfigurationWidget *ui;
};

#endif // SPHINXTESTCONFIGURATIONWIDGET_H
