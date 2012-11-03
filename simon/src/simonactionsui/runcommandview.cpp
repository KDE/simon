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

#include "runcommandview.h"
#include "runcommandviewprivate.h"
#include <KLocalizedString>

/**
 *	@brief Constructor
 *
 *	@author Peter Grasch
 */
RunCommandView::RunCommandView(QWidget* parent)
: InlineWidget(i18n("Commands"),
KIcon("system-run"),
i18n("Direct Execution of Simon Commands"), parent),
d(new RunCommandViewPrivate(this))
{
  connect(d, SIGNAL(actionsChanged()), this, SIGNAL(actionsChanged()));
  QVBoxLayout *lay = new QVBoxLayout(this);
  lay->addWidget(d);

  hide();
}


void RunCommandView::displayScenarioPrivate(Scenario *scenario)
{
  d->displayScenario(scenario);
}


/**
 *	@brief Destructor
 *
 *	@author Peter Grasch
 */
RunCommandView::~RunCommandView()
{
  d->deleteLater();
}
