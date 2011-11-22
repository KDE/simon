/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_VOCABULARYVIEW_H_47AEB5F97F5246E8BAC891DD0B65687A
#define SIMON_VOCABULARYVIEW_H_47AEB5F97F5246E8BAC891DD0B65687A

#include <simonuicomponents/inlinewidget.h>
#include "simonmodelmanagementui_export.h"

#include <simonscenarios/scenariodisplay.h>

class VocabularyViewPrivate;

/**
 *	@class VocabularyView
 *	@brief Frontend to the Wordlist
 *
 *	@version 0.1
 *	@date 23.01.2006
 *	@author Peter Grasch
 */
class SIMONMODELMANAGEMENTUI_EXPORT VocabularyView : public InlineWidget, public ScenarioDisplay
{
  Q_OBJECT

  public:
    VocabularyView(QWidget *parent=0);

  protected:
    void displayScenarioPrivate(Scenario *scenario);

  private:
    VocabularyViewPrivate *d;
};
#endif
