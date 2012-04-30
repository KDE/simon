/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
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

class QShowEvent;
class QHideEvent;
class VolumeWidget;

class WelcomePage : public InlineWidget, public ScenarioDisplay
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
      
    void displayScenarios();
    void displayAcousticModelInfo();
    
    void updateScenarioDisplays();
    
private:
    Ui::WelcomePage ui;
    VolumeWidget *volumeWidget;
    QString getCurrentlySelectedScenarioId();
};

#endif // WELCOMEPAGE_H
