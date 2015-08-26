/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#include "dialogrunner.h"
#include "akonadiconfiguration.h"
#include <simondialogengine/dialogtextparser.h>
#include <simondialogengine/dialogcommand.h>
#include <simondialogengine/dialogstate.h>
#include <simondialogengine/graphicalview/visualdialogview.h>
#include <simondialogengine/ttsview/ttsdialogview.h>
#include <QDebug>
#include <QTimer>

bool DialogRunner::greedyTrigger(const QString& sentence)
{
  QList<DialogCommand*> commands = m_state->getTransitions();
  bool found = false;
  foreach (DialogCommand* c, commands) {
    if (c->matches(0, sentence)) {
      found = c->trigger(0);
    }
  }
  
  if (!found)
  {
    //check if this is a repeat trigger
    if (m_config->getRepeatTriggers().contains(sentence, Qt::CaseInsensitive))
    {
      foreach (DialogView* view, m_dialogViews)
        view->repeat(*m_state);
      found = true;
    }
  }
  if (found) {
    foreach (DialogView* view, m_dialogViews)
      view->correctInputReceived();
  } else {
    foreach (DialogView* view, m_dialogViews)
      view->warnOfInvalidInput(sentence);
  }
  return GreedyReceiver::greedyTrigger(sentence);
}

bool DialogRunner::getRepeatOnInvalidInput() const
{
  return m_config->getRepeatOnInvalidInput();
}

QString DialogRunner::getRepeatAnnouncement() const
{
  return m_config->getRepeatAnnouncement();
}

QString DialogRunner::getOptionSeparatorText() const
{
  return m_config->getOptionSeparatorText();
}

bool DialogRunner::getDisplayAvatarNames() const
{
  return m_config->getDisplayAvatarNames();
}

int DialogRunner::getAvatarSize() const
{
  return m_config->getAvatarSize();
}

Avatar* DialogRunner::getAvatar(int id) const
{
  return m_config->getAvatar(id);
}

void DialogRunner::initState(int state)
{
  qDebug() << "Switching to state: " << state;

  bool quit = false;
  //0 state means quit
  switch (state) {
    case 1:
      initState(m_state);
      break;
    case 2:
      qDebug() << "dismissing";
      deleteLater();
      quit = true;
      break;
    case 3:
      qDebug() << "reminding later";
      quit = true;
      qDebug() << "Restarting in: " << m_config->getRestartDelay()*1000;
//       run();
//       QTimer::singleShot(10, this, SLOT(run()));
      QTimer::singleShot(m_config->getRestartDelay()*1000, this, SLOT(run()));
      break;
    default:
      quit = true;
      deleteLater();
      break;
  }
  if (quit) {
    foreach (DialogView* view, m_dialogViews)
      view->stop();

    stopGreedy();
//     deleteLater();
  }
}

void DialogRunner::initState(DialogState* state)
{
  foreach (DialogView* view, m_dialogViews)
    view->present(*state);

  state->presented();
}

DialogRunner::DialogRunner(AkonadiConfiguration* config, DialogTextParser* parser, DialogState* state): GreedyReceiver(0 /* no parent manager*/),
  m_config(config),  m_dialogParser(parser), m_state(state)
{
  connect(state, SIGNAL(requestDialogState(int)), this, SLOT(initState(int)));
}

void DialogRunner::run()
{
  qDeleteAll(m_dialogViews);
  m_dialogViews.clear();

  if (!m_state)
    return;

  if (m_config->useGUIOutput())
    m_dialogViews << new VisualDialogView(this);
  if (m_config->useTTSOutput())
    m_dialogViews << new TTSDialogView(this);
  
  if (m_dialogViews.isEmpty()) return;

  foreach (DialogView* view, m_dialogViews)
    view->start();

  startGreedy();
  
  initState(1); // always start with state 1;
}

DialogRunner::~DialogRunner()
{
  qDeleteAll(m_dialogViews);
  delete m_dialogParser;
  
  qDeleteAll(m_state->getTransitions());
  m_state->deleteLater();
}

