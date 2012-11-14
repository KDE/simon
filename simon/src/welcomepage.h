/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef SIMON_WELCOMEPAGE_H_47AEB5F97F5246E8BAC891DD0B65687A
#define SIMON_WELCOMEPAGE_H_47AEB5F97F5246E8BAC891DD0B65687A

#include <simonuicomponents/inlinewidget.h>
#include <simonscenarios/scenariodisplay.h>
#include <simonrecognitionresult/recognitionresult.h>
#include "ui_welcomepage.h"

class TrainingTextAggregatorModel;
class QShowEvent;
class QHideEvent;
class VolumeWidget;
class QModelIndex;
class QTreeWidgetItem;

class WelcomePage : public QWidget, public ScenarioDisplay
{
Q_OBJECT

signals:
    void editScenario();
  
public:
    WelcomePage( QAction* activationAction, QWidget* parent = 0 );
    
protected:
    void showEvent(QShowEvent* event);
    void hideEvent(QHideEvent* event);
    virtual void displayScenarioPrivate(Scenario *scenario);
    
private slots:
    void processedRecognitionResult(const RecognitionResult& recognitionResult, bool accepted);
    
    void baseModelConfig();
    void audioConfig();
    void scenarioConfig();
      
    void displayScenario(QTreeWidgetItem* parent, Scenario *s);
    void displayScenarios();
    void displayAcousticModelInfo();
    
    void updateScenarioDisplays();
    
    void updateTrainingsTexts();
    void startTraining();
    void manageSamples();
    
    void trainingsTextSelected(const QModelIndex& index);
    
private:
    Ui::WelcomePage ui;
    VolumeWidget *volumeWidget;
    TrainingTextAggregatorModel *trainingTextModel;
    QString getCurrentlySelectedScenarioId();

    QTreeWidgetItem* findScenario(const QString& id) const;
};

#endif // WELCOMEPAGE_H
