#ifndef JULIUSTESTCONFIGURATIONWIDGET_H
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

#define JULIUSTESTCONFIGURATIONWIDGET_H

#include "testconfigurationwidget.h"
#include <KUrl>

namespace Ui {
class JuliusTestConfigurationWidget;
}

class JuliusTestConfigurationWidget : public TestConfigurationWidget
{
  Q_OBJECT
  
public:
  explicit JuliusTestConfigurationWidget(QWidget *parent = 0);
  JuliusTestConfigurationWidget(CorpusInformation* corpusInfo,
                               const KUrl& testPromptsUrl, const KUrl& testPromptsBasePathUrl,
                               int sampleRate, QWidget *parent=0);

  ~JuliusTestConfigurationWidget();

  void init(const QHash<QString, QString>&params);

  KUrl hmmDefs() const;
  KUrl tiedlist() const;
  KUrl dict() const;
  KUrl dfa() const;
  KUrl jconf() const;

  QDomElement serialize(QDomDocument *doc);

  BackendType getBackendType() const { return TestConfigurationWidget::JHTK; }
  
protected:
  void changeEvent(QEvent *e);
  void setupUi();

protected slots:
  void update();
  
private:
  Ui::JuliusTestConfigurationWidget *ui;
};

#endif // JULIUSTESTCONFIGURATIONWIDGET_H
