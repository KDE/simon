/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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
#include "dialogstate.h"
#include <simontts/simontts.h>
#include <KDebug>
#include <KLocale>
#include <KMessageBox>

TTSDialogView::TTSDialogView(DialogCommandManager *dialog) :
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

  if (!SimonTTS::uninitialize())
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
    KMessageBox::sorry(0, i18n("Could not speak text: %1", text));
    return false;
  }
  return true;
}

bool TTSDialogView::present(const DialogState& state)
{
  kDebug() << "Presenting state in tts dialog view...";

  bool succ = true;
  succ &= say(state.getText());

  /*
  QList<DialogCommand*> transitions = state.getTransitions();

  kDebug() << "Presenting " << transitions.count() << " transitions";

  foreach (DialogCommand* transition, transitions)
  {
    DialogCommandButton *button = new DialogCommandButton(transition);
    static_cast<QVBoxLayout*>(ui->wgAnswers->layout())->addWidget(button);
    m_buttons << button;
  }
  */

  return succ;
}


void TTSDialogView::setFont(const QFont& font)
{
  Q_UNUSED(font);
}

