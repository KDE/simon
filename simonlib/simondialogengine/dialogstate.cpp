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

#include "dialogstate.h"
#include "dialogcommand.h"
#include "dialogtext.h"
#include "dialogturn.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDateTime>

DialogState::DialogState(DialogTextParser* parser, const QString& name, QList<DialogTurn*> turns, QObject* parent) :
  QAbstractItemModel(parent),
  m_name(name),
  m_currentRandomTurnIndex(0), /* only one turn */
  m_parser(parser),
  //m_silence(silence),
  //m_announceRepeat(announceRepeat),
  m_turns(turns)
  //m_transitions(transitions)
{
  qsrand(QDateTime::currentDateTime().toTime_t());

  DialogTurn* turn = new DialogTurn(parser, name, QString(), false, true, QList<DialogCommand*>(), this);
  //connect(turn, SIGNAL(requestDialogTurn(int)), this, SLOT(initState(int)));
  connect(turn, SIGNAL(changed()), this, SLOT(TurnChanged()));
  m_turns << turn;

  /*
  foreach (DialogCommand *c, m_transitions)
  {
    kDebug() << "Registering dialog command: " << c->getTrigger();
    connect(c, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
    connect(c, SIGNAL(changed()), this, SIGNAL(changed()));
  }
  */
}

DialogState* DialogState::createInstance(DialogTextParser *parser, const QDomElement& elem)
{
  DialogState* state = new DialogState();

  if (!state->deSerialize(parser, elem))
  {
    delete state;
    return NULL;
  }

  return state;
}

void DialogState::addTurn(const QString& name, DialogTextParser* dialogParser)
{
  DialogTurn *turn = new DialogTurn(dialogParser, name, QString(), false, true,
                                      QList<DialogCommand*>(), this);
  //connect(turn, SIGNAL(requestDialogState(int)), this, SLOT(initState(int)));
  connect(turn, SIGNAL(changed()), this, SLOT(TurnChanged()));
  m_turns << turn;
}

int DialogState::getTextCount()
{
  int count = 0;
  foreach(DialogTurn* t, m_turns)
  {
    count += t->getTextCount();
  }
  return count;
}

int DialogState::addText(const QString& text)
{
  return m_turns.at(0)->addText(text);
  /*
  m_texts << new DialogText(m_parser, text);
  updateRandomTextSelection();
  return m_texts.count() - 1;
  */
}

bool DialogState::removeText(int id)
{
  return m_turns.at(0)->removeText(id);
  /*
  if (id >= m_texts.count())
    return false;
  
  delete m_texts.takeAt(id);
  updateRandomTextSelection();
  return true;
  */
}

void DialogState::updateRandomTextSelection()
{
  //yeah, yeah non-even distribution and predictable randoms on old implementations..
  //who cares for this purpose :)
  m_turns.at(0)->updateRandomTextSelection();
}

QString DialogState::getText() const
{
  return m_turns.at(0)->getText();
  //return m_texts[m_currentRandomTextIndex]->parse();
}

QString DialogState::getRawText(int index) const
{
  return m_turns.at(0)->getRawText(index);
  //return m_texts[index]->source();
}

bool DialogState::setRawText(int index, const QString& data)
{
  return m_turns.at(0)->setRawText(index, data);
  /*
  m_texts[index]->setSource(data);
  emit changed();
  return true;
  */
}

void DialogState::presented()
{
  /*
  foreach (DialogCommand *c, m_transitions)
    c->presented();
  */
  m_turns.at(0)->presented();
}

void DialogState::left()
{
  /*
  foreach (DialogCommand *c, m_transitions)
    c->left();
  */
  m_turns.at(0)->left();
}

bool DialogState::deSerialize(DialogTextParser *parser, const QDomElement& elem)
{
  return true;
  /*
  if (elem.isNull()) return false;

  m_name = elem.attribute("name");
  m_parser = parser;

  //qDeleteAll(m_texts);
  //m_texts.clear();

  qDeleteAll(m_turns);
  m_turns.clear();
  
  QList m_texts = QList<DialogText*>();
  QList m_transitions = QList<DialogCommand*>();

  QDomElement text = elem.firstChildElement("text");
  do
  {
    m_texts << new DialogText(parser, text.text());
    text = text.nextSiblingElement("text");
  } while (!text.isNull());
  
  updateRandomTextSelection();
  
  QDomElement textOptions = elem.firstChildElement("textOptions");
  QDomElement textSilenceOption = textOptions.firstChildElement("silence");
  QDomElement textAnnounceRepeatOption = textOptions.firstChildElement("announceRepeat");
  
  m_silence = (textSilenceOption.text() == "1");
  m_announceRepeat = (textAnnounceRepeatOption.text() == "1");
  
  QDomElement avatarElem = elem.firstChildElement("avatar");
  m_displayAvatar = (avatarElem.attribute("enabled") == "1");
  m_avatarId = avatarElem.text().toInt();

  QDomElement transitions = elem.firstChildElement("transitions");
  QDomElement transition = transitions.firstChildElement("command");

  QList<DialogCommand*> commands;
    kDebug() << "Deserializing transition: setup";
  while (!transition.isNull())
  {
    kDebug() << "Deserializing transition";
    DialogCommand *c = DialogCommand::createInstance(transition);
    if (c)
    {
      connect(c, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
      connect(c, SIGNAL(changed()), this, SIGNAL(changed()));
      commands << c;
    } else 
      kDebug() << "FAILED";

    transition = transition.nextSiblingElement("command");
  }

  m_transitions = commands;
  return true;
  */
}

QDomElement DialogState::serialize(QDomDocument *doc)
{
  /*
  QDomElement elem = doc->createElement("state");
  elem.setAttribute("name", m_name);

  foreach (DialogText *t, m_texts)
  {
    QDomElement textElem = doc->createElement("text");
    textElem.appendChild(doc->createTextNode(t->source()));
    elem.appendChild(textElem);
  }
  QDomElement textOptions = doc->createElement("textOptions");
  QDomElement textSilenceOption = doc->createElement("silence");
  QDomElement textAnnounceOption = doc->createElement("announceRepeat");

  textSilenceOption.appendChild(doc->createTextNode(m_silence ? "1" : "0"));
  textAnnounceOption.appendChild(doc->createTextNode(m_announceRepeat ? "1" : "0"));

  textOptions.appendChild(textSilenceOption);
  textOptions.appendChild(textAnnounceOption);
  
  QDomElement avatarElem = doc->createElement("avatar");
  avatarElem.setAttribute("enabled", m_displayAvatar ? "1" : "0");
  avatarElem.appendChild(doc->createTextNode(QString::number(m_avatarId)));

  QDomElement transitionsElem = doc->createElement("transitions");
  
  foreach (DialogCommand *c, m_transitions)
    transitionsElem.appendChild(c->serialize(doc));

  elem.appendChild(textOptions);
  elem.appendChild(avatarElem);
  elem.appendChild(transitionsElem);

  return elem;
  */
  return m_turns.at(0)->serialize(doc);
}

void DialogState::addTransition(DialogCommand* command)
{
  /*
  beginInsertRows(QModelIndex(), m_transitions.count(), m_transitions.count());

  m_transitions << command;
  connect(command, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
  emit changed();

  endInsertRows();
  */
  m_turns.at(0)->addTransition(command);
}

void DialogState::removeTransition(DialogCommand* command)
{
  /*
  for (int i=0; i < m_transitions.count(); i++) {
    if (m_transitions.at(i) == command) {
      beginRemoveRows(QModelIndex(), i, i);
      m_transitions.removeAt(i);
      endRemoveRows();

      //will be deleted in the command manager
      //delete command;
    }
  }
  emit changed();
  */
  m_turns.at(0)->removeTransition(command);
}

bool DialogState::moveTransitionUp(DialogCommand* command)
{
  /*
  int i = m_transitions.indexOf(command);
  if (i <= 0) return false;

  m_transitions.insert(i-1, m_transitions.takeAt(i));
  emit changed();
  emit dataChanged(index(i-1, 0),index(i, 0));
  return true;
  */
  return m_turns.at(0)->moveTransitionUp(command);
}

bool DialogState::moveTransitionDown(DialogCommand* command)
{
  /*
  int i = m_transitions.indexOf(command);
  if ((i == -1) || (i == (m_transitions.count()-1))) 
    return false;

  m_transitions.insert(i+1, m_transitions.takeAt(i));
  emit changed();
  emit dataChanged(index(i, 0),index(i+1, 0));
  return true;
  */
  return m_turns.at(0)->moveTransitionDown(command);
}

bool DialogState::rename(const QString& name)
{
  m_name = name;
  emit changed();
  return true;
}


Qt::ItemFlags DialogState::flags(const QModelIndex &index) const
{
  /*
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
  */
  return m_turns.at(0)->flags(index);
}

QVariant DialogState::headerData(int column, Qt::Orientation orientation,
                  int role) const
{
  /*
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18nc("Name of the state", "Name");
    }
  }

  //default
  return QVariant();
  */
  return m_turns.at(0)->headerData(column, orientation, role);
}

QModelIndex DialogState::parent(const QModelIndex &index) const
{
  /*
  Q_UNUSED(index);
  return QModelIndex();
  */
  return m_turns.at(0)->parent(index);
}

int DialogState::rowCount(const QModelIndex &parent) const
{
  /*
  Q_UNUSED(parent);
  return m_transitions.count();
  */
  return m_turns.at(0)->rowCount(parent);
}


QModelIndex DialogState::index(int row, int column, const QModelIndex &parent) const
{
  /*
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();


   return createIndex(row, column, m_transitions[row]);
  */
  return m_turns.at(0)->index(row, column, parent);
}

QVariant DialogState::data(const QModelIndex &index, int role) const
{
  /*
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole)
    return m_transitions.at(index.row())->getTrigger();

  if (role == Qt::DecorationRole)
    return m_transitions.at(index.row())->getIcon();

  return QVariant();
  */
  return m_turns.at(0)->data(index, role);
}

int DialogState::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return 1;
}

void DialogState::setSilence(bool silence)
{
  m_turns.at(0)->setSilence(silence);
}

void DialogState::setAnnounceRepeat(bool announce)
{
  m_turns.at(0)->setAnnounceRepeat(announce);
}

DialogState::~DialogState()
{
  qDeleteAll(m_turns);
}

/*
void DialogCommandManager::initState(DialogTurn* turn)
{
  if (currentDialogTurn)
    currentDialogTurn->left();

  turn->updateRandomTextSelection();

  foreach (DialogView* view, dialogViews)
    view->present(*turn);

  turn->presented();

  currentDialogTurn = turn;
}

bool DialogCommandManager::addTurn(const QString& name)
{
  DialogTurn *turn = new DialogTurn(dialogParser, name, QString(), false, true,
                                    QList<DialogCommand*>(), this);
  connect(turn, SIGNAL(requestDialogTurn(int)), this, SLOT(initState(int)));
  connect(turn, SIGNAL(changed()), this, SLOT(StateChanged()));
  DialogTurns << turn;
  kDebug() << "Adding turn...";

  return true;
  //return parentScenario->save();
}

bool DialogCommandManager::removeTurn(DialogTurn *turn)
{
  if (turn == currentDialogTurn)
  {
    currentDialogTurn = NULL;
    initState(0);
  }
  int removed = DialogTurns.removeAll(turn);

  if (!removed)
    return false;

  delete turn;
  return true;
  //return parentScenario->save();
}

bool DialogCommandManager::moveTurnUp(DialogTurn *turn)
{
  int index = DialogTurns.indexOf(turn);
  if (index <= 0) return false;

  DialogTurns.insert(index-1, DialogTurns.takeAt(index));
  return true;
  //return parentScenario->save();
}

bool DialogCommandManager::moveTurnDown(DialogTurn *turn)
{
  int index = DialogTurns.indexOf(turn);
  if ((index == -1) || (index == (DialogTurns.count()-1)))
    return false;

  DialogTurns.insert(index+1, DialogTurns.takeAt(index));
  return true;
  //return parentScenario->save();
}


QDomElement DialogCommandManager::serializeCommands(QDomDocument *doc)
{
  QDomElement commandsElem = doc->createElement("commands");
  foreach (Command *c, commands) {
    //only store voice interface commands
    if (dynamic_cast<VoiceInterfaceCommand*>(c))
    {
      QDomElement commandElem = c->serialize(doc);
      commandElem.setTagName("voiceInterfaceCommand");
      commandsElem.appendChild(commandElem);
    }
  }

  foreach (DialogTurn *turn, DialogTurns) {
    QDomElement commandElem = turn->serialize(doc);
    commandsElem.appendChild(commandElem);
  }

  return commandsElem;
}

bool DialogCommandManager::deSerializeCommandsPrivate(const QDomElement& elem)
{
  if (elem.isNull()) return false;

  QDomElement stateElem = elem.firstChildElement("State");
  while(!stateElem.isNull())
  {
    kDebug() << "Deserializing State element";
    DialogTurn *turn = DialogTurn::createInstance(dialogParser, stateElem);

    if (turn)
    {
      connect(turn, SIGNAL(requestDialogTurn(int)), this, SLOT(initState(int)));
      connect(turn, SIGNAL(changed()), this, SLOT(StateChanged()));
      connect(turn, SIGNAL(destroyed()), this, SLOT(StateDestroyed()));
      DialogTurns << turn;
    }

    stateElem = stateElem.nextSiblingElement("State");
  }

  bindStateCommands();

  getDialogConfiguration()->init();
  return true;
}

void DialogCommandManager::StateDestroyed()
{
  DialogTurns.removeAll(static_cast<DialogTurn*>(sender()));
}

void DialogCommandManager::bindStateCommands()
{
  kDebug() << "rebinding";
  QList<Command*> oldCommands;

  foreach (Command* c, commands)
  {
    if (dynamic_cast<DialogCommand*>(c))
    {
      commands.removeAll(c);
      oldCommands << c;
    }
  }

  int StateId = SimonCommand::GreedyState + 1;
  foreach (DialogTurn *turn, DialogTurns)
  {
    QList<DialogCommand*> transitions = turn->getTransitions();

    foreach (DialogCommand* transition, transitions)
    {
      transition->createStateLink(StateId);
      commands << transition;
    }

    StateId++;
  }

  foreach (Command* c, oldCommands)
  {
    if (!commands.contains(c))
      delete c;
  }
}

DialogCommandManager::~DialogCommandManager()
{
  activateAction->deleteLater();
  qDeleteAll(dialogViews);
  foreach (DialogTurn *s, DialogTurns)
    disconnect(s, SIGNAL(destroyed()), this, SLOT(StateDestroyed()));
  qDeleteAll(DialogTurns);
}
*/

void DialogState::bindStateCommands(QList<Command*> commands)
{
  int StateId = SimonCommand::GreedyState + 1;
  foreach (DialogTurn *turn, m_turns)
  {
    QList<DialogCommand*> transitions = turn->getTransitions();

    foreach (DialogCommand* transition, transitions)
    {
      transition->createStateLink(StateId);
      commands << transition;
    }

    StateId++;
  }
}

bool DialogState::silence() const { return m_turns.at(0)->silence(); }
bool DialogState::announceRepeat() const { return m_turns.at(0)->announceRepeat(); }

bool DialogState::getDisplayAvatar() const { return m_turns.at(0)->getDisplayAvatar(); }
void DialogState::setDisplayAvatar(bool display) { m_turns.at(0)->setDisplayAvatar(display); }

int DialogState::getAvatarId() const { return m_turns.at(0)->getAvatarId(); }
void DialogState::setAvatar(int id) { m_turns.at(0)->setAvatar(id); }

QList<DialogCommand*> DialogState::getTransitions() const
{
  return m_turns.at(0)->getTransitions();
}

void DialogState::TurnChanged()
{
  emit changed();
}

void DialogState::TurnDestroyed()
{
  emit destroyed();
}

