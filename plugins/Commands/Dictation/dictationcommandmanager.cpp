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
#include "dictationcommandmanager.h"
#include "replacements.h"
#include "dictationconfiguration.h"
#include <eventsimulation/eventhandler.h>
#include <simoninfo/simoninfo.h>
#include <KLocalizedString>

K_PLUGIN_FACTORY( DictationCommandPluginFactory,
registerPlugin< DictationCommandManager >();
)

K_EXPORT_PLUGIN( DictationCommandPluginFactory("simondictationcommand") )

DictationCommandManager::DictationCommandManager(QObject* parent, const QVariantList& args) :
  CommandManager((Scenario*) parent, args)
{
}


bool DictationCommandManager::trigger(const QString& triggerName, bool silent)
{
  Q_UNUSED(silent);

  //voice interface commands
  if (CommandManager::trigger(triggerName, silent))
    return true;

  DictationConfiguration *c = static_cast<DictationConfiguration*>(config);
  QString appendText = c->appendText();
  QString myText = c->replacements()->replace(triggerName);
  if (c->upperCaseFirst())
    myText = myText.left(1).toUpper() + myText.mid(1);
  EventHandler::getInstance()->sendWord(myText+appendText);
  return true;
}

void DictationCommandManager::selectText(const QString& text)
{
  kDebug() << "Selecting: " << text;

  #ifdef ATSPI_ENABLED
  if (m_currentInputField.isValid()) {
    QString currentText = m_currentInputField.text();
    int lastIndex = currentText.lastIndexOf(text);
    if (lastIndex == -1) {
      SimonInfo::showMessage(i18n("No such text: %1", text), 2500);
      return;
    }
    int endIndex = text.length() + lastIndex;
    m_currentInputField.setTextSelections(QList<QPair<int, int> >() << qMakePair(lastIndex, endIndex));
//     m_currentInputField.setCaretOffset(30);
  } else
    kDebug() << "No current input field";
  #endif
}

bool DictationCommandManager::setupCommands()
{
  bool succ = true;

  succ &= installInterfaceCommand(this, "selectText", i18n("Select %%1"), "document-edit",
    i18n("Selects the given text in the currently edited text.\n\nIf ambiguous, the last matching sequence is selected"),
    true /* announce */, true /* show icon */,
    SimonCommand::DefaultState /* consider this command when in this state */,
    SimonCommand::DefaultState, /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "selectText" /* id */);
  #ifdef ATSPI_ENABLED
  kDebug() << "ATSPI dictation enabled";
  #endif
  return succ;
}

const QString DictationCommandManager::name() const
{
  return i18n("Dictation");
}


const QString DictationCommandManager::iconSrc() const
{
  return ("text-field");
}


bool DictationCommandManager::deSerializeConfig(const QDomElement& elem)
{
  config = new DictationConfiguration(parentScenario);
  if (!setupCommands()) {
    kWarning() << "Failed to setup interface commands";
  }

  #ifdef ATSPI_ENABLED
  m_registry = new QAccessibleClient::Registry(this);
  m_registry->applications(); // FIXME: KDE bug: 307264
  connect(m_registry, SIGNAL(focusChanged(QAccessibleClient::AccessibleObject)), this, SLOT(focusChanged(QAccessibleClient::AccessibleObject)));
  m_registry->subscribeEventListeners(QAccessibleClient::Registry::Focus);
  #endif
  return config->deSerialize(elem);
}

#ifdef ATSPI_ENABLED
void DictationCommandManager::focusChanged(const QAccessibleClient::AccessibleObject &object)
{
  if (object.supportedInterfaces() & QAccessibleClient::AccessibleObject::TextInterface) {
    m_currentInputField = object;
  } else {
    if (m_currentInputField.isValid()) {
      kDebug() << "No current input field";
      m_currentInputField = QAccessibleClient::AccessibleObject();
    }
    else
      kDebug() << "Ignoring change to " << object.name();
  }
}
#endif

DictationCommandManager::~DictationCommandManager()
{
}
