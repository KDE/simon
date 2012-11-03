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

#ifndef SIMON_TRAINSAMPLEPAGE_H_9BFB421E7D5244CEA038BB68E638C169
#define SIMON_TRAINSAMPLEPAGE_H_9BFB421E7D5244CEA038BB68E638C169

#include <QString>
#include <QWizardPage>
#include <simonscenarios/promptstable.h>
class RecWidget;

class TrainSamplePage : public QWizardPage
{
  Q_OBJECT

  private:
    RecWidget *recorder;
    QString prompt;
    QString fileName;

    QStringList getFileNames() const;

  public:
    TrainSamplePage(QString prompt, int nowPage, int maxPage, const QString name, QWidget *parent=0);
    ~TrainSamplePage();
    bool isComplete() const;

    void initializePage();
    bool validatePage();
    void cleanupPage();

    PromptsTable getPrompts() const;
    QString getPrompt() const { return prompt; }

  public slots:
    bool submit();
    bool cleanUp();

};
#endif
