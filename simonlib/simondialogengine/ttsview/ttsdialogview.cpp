/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "ttsdialogview.h"
#include <simondialogengine/dialogmanager.h>
#include <simondialogengine/dialogturn.h>
#include <simondialogengine/dialogcommand.h>
#include <simontts/simontts.h>
#include <KDebug>
#include <KLocale>
#include <KMessageBox>

TTSDialogView::TTSDialogView(DialogManager *dialog) :
  DialogView(dialog)
{
}

bool TTSDialogView::start()
{
  kDebug() << "Starting tts dialog view...";

  if (!SimonTTS::initialize())
  {
    KMessageBox::sorry(0, i18n("Failed to initialize text to speech subsystem"));
    return false;
  }

  return true;
}

bool TTSDialogView::stop()
{
  kDebug() << "Stopping tts dialog view...";

  if (!SimonTTS::interrupt() || !SimonTTS::uninitialize())
  {
    KMessageBox::information(0, i18n("Failed to uninitialize text to speech subsystem"));
    return false;
  }

  return true;
}

bool TTSDialogView::say(const QString& text)
{
  if (!SimonTTS::say(text))
  {
    KMessageBox::sorry(0, i18nc("%1 is errenous text", "Could not speak text: %1", text));
    return false;
  }
  return true;
}

bool TTSDialogView::synthesizeTurn(const DialogTurn& turn)
{
  QString text;
  
  if (!turn.silence())
    text += turn.getText()+'\n';
  
  optionsRepeat = "";
  

  QList<DialogCommand*> transitions = turn.getTransitions();

  kDebug() << "Presenting " << transitions.count() << " transitions";
  foreach (DialogCommand* transition, transitions)
  {
    if (!transition->silent())
      optionsRepeat += transition->getTrigger()+".\n";
  }
  if (!optionsRepeat.trimmed().isEmpty())
  {
    optionsRepeat.insert(0, m_dialog->getOptionSeparatorText()+'\n');
    text += optionsRepeat;
  }

  if (turn.announceRepeat())
    text += m_dialog->getRepeatAnnouncement();
    //say(m_dialog->getRepeatAnnouncement());

  if (text.isEmpty())
  {
    kDebug() << "Nothing to say";
    return true;
  }
  kDebug() << "Saying text: " << text;

  return say(text);
}

bool TTSDialogView::present(const DialogTurn& turn)
{
  kDebug() << "Presenting turn in tts dialog view...";
  if (!SimonTTS::interrupt()) return false;

  return synthesizeTurn(turn);
}

void TTSDialogView::repeat(const DialogTurn& turn)
{
  synthesizeTurn(turn);
}

void TTSDialogView::warnOfInvalidInput(const QString& /*input*/)
{
  if (m_dialog->getRepeatOnInvalidInput())
    say(optionsRepeat);
}

TTSDialogView::~TTSDialogView()
{
}

