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

#ifndef SIMON_DICTATIONCONFIGURATION_H_DF2B936F15A84FD8962EF68E1DAFE24F
#define SIMON_DICTATIONCONFIGURATION_H_DF2B936F15A84FD8962EF68E1DAFE24F

#include <simonscenarios/commandconfiguration.h>
#include "ui_dictationconfigurationdlg.h"
#include <KSharedConfig>
#include <QPointer>

class Replacement;
class Replacements;
class DictationConfiguration : public CommandConfiguration
{
Q_OBJECT

public slots:
  virtual bool deSerialize(const QDomElement&);
  virtual QDomElement serialize(QDomDocument *doc);
  virtual void defaults();

public:
  explicit DictationConfiguration(Scenario *parent, const QVariantList &args = QVariantList());
  ~DictationConfiguration();

  //configuration options
  bool upperCaseFirst() const;
  QString appendText() const;
  Replacements* replacements() const;

private slots:
  void addReplacement();
  void editReplacement();
  void removeReplacement();

private:
  Ui::DictationConfigurationDlg ui;
  Replacements *m_replacements;
  Replacement* getCurrentlySelectedRecommendation() const;
};
#endif
