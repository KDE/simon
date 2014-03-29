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

  // DialogTurn* turn = new DialogTurn(parser, name, QString(), false, true, QList<DialogCommand*>(), this);
  //connect(turn, SIGNAL(requestDialogTurn(int)), this, SLOT(initState(int)));
  // connect(turn, SIGNAL(changed()), this, SLOT(turnChanged()));
  // m_turns << turn;

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

DialogTurn* DialogState::createTurn()
{
  DialogTurn *turn = new DialogTurn(m_parser, QString(), QString(), false, true,
                                      QList<DialogCommand*>(), this);
  return turn;
}

void DialogState::addTurn(DialogTurn* turn)
{
  connect(turn, SIGNAL(changed()), this, SLOT(turnChanged()));
  m_turns << turn;
  currentDialogTurn = turn;
}

void DialogState::removeTurn(int index)
{
  DialogTurn* turn = m_turns[index];
  m_turns.removeAll(turn);
  delete turn;
}

void DialogState::setTurn(DialogTurn* turn, int index)
{
  DialogTurn* old = m_turns[index];
  m_turns[index] = turn;
  connect(turn, SIGNAL(changed()), this, SLOT(turnChanged()));
  delete old;
}

int DialogState::getTextCount()
{
  // int count = 0;
  // foreach(DialogTurn* t, m_turns)
  // {
  //   count += t->getTextCount();
  // }
  // return count;
  return currentDialogTurn->getTextCount();
}

int DialogState::addText(const QString& text)
{
  return currentDialogTurn->addText(text);
  // return m_turns.at(0)->addText(text);
  /*
  m_texts << new DialogText(m_parser, text);
  updateRandomTextSelection();
  return m_texts.count() - 1;
  */
}

bool DialogState::removeText(int id)
{
  return currentDialogTurn->removeText(id);
  // return m_turns.at(0)->removeText(id);
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
  // m_turns.at(0)->updateRandomTextSelection();
  currentDialogTurn->updateRandomTextSelection();
}

QString DialogState::getText() const
{
  return currentDialogTurn->getText();
  // return m_turns.at(0)->getText();
  //return m_texts[m_currentRandomTextIndex]->parse();
}

QString DialogState::getRawText(int index) const
{
  return currentDialogTurn->getRawText(index);
  // return m_turns.at(0)->getRawText(index);
  //return m_texts[index]->source();
}

bool DialogState::setRawText(int index, const QString& data)
{
  return currentDialogTurn->setRawText(index, data);
  // return m_turns.at(0)->setRawText(index, data);
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
  // m_turns.at(0)->presented();
  currentDialogTurn->presented();
}

void DialogState::left()
{
  /*
  foreach (DialogCommand *c, m_transitions)
    c->left();
  */
  // m_turns.at(0)->left();
  currentDialogTurn->left();
}

bool DialogState::deSerialize(DialogTextParser *parser, const QDomElement& elem)
{
  if (elem.isNull()) return false;

  m_name = elem.attribute("name");
  m_parser = parser;

  qDeleteAll(m_turns);
  m_turns.clear();

  QDomElement turns = elem.firstChildElement("turns");
  QDomElement turn = turns.firstChildElement("turn");

  kDebug() << "Deserializing turn: setup";
  while (!turn.isNull())
  {
    kDebug() << "Deserializing turn";
    DialogTurn* t = DialogTurn::createInstance(parser, turn);
    if (t)
    {
      connect(t, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
      connect(t, SIGNAL(changed()), this, SIGNAL(changed()));
      m_turns << t;
    } else 
      kDebug() << "FAILED";

    turn = turn.nextSiblingElement("turn");
  }

  return true;
}

QDomElement DialogState::serialize(QDomDocument *doc)
{
  
  QDomElement elem = doc->createElement("state");
  elem.setAttribute("name", m_name);

  QDomElement turnsElem = doc->createElement("turns");
  
  foreach (DialogTurn* t, m_turns)
    turnsElem.appendChild(t->serialize(doc));

  elem.appendChild(turnsElem);

  return elem;
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
  // m_turns.at(0)->addTransition(command);
  currentDialogTurn->addTransition(command);
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
  // m_turns.at(0)->removeTransition(command);
  currentDialogTurn->addTransition(command);
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
  // return m_turns.at(0)->moveTransitionUp(command);
  return currentDialogTurn->moveTransitionUp(command);
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
  // return m_turns.at(0)->moveTransitionDown(command);
  return currentDialogTurn->moveTransitionDown(command);
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
  // return m_turns.at(0)->flags(index);
  return currentDialogTurn->flags(index);
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
  // return m_turns.at(0)->headerData(column, orientation, role);
  return currentDialogTurn->headerData(column, orientation, role);
}

QModelIndex DialogState::parent(const QModelIndex &index) const
{
  /*
  Q_UNUSED(index);
  return QModelIndex();
  */
  // return m_turns.at(0)->parent(index);
  return currentDialogTurn->parent(index);
}

int DialogState::rowCount(const QModelIndex &parent) const
{
  /*
  Q_UNUSED(parent);
  return m_transitions.count();
  */
  // return m_turns.at(0)->rowCount(parent);
  return currentDialogTurn->rowCount(parent);
}


QModelIndex DialogState::index(int row, int column, const QModelIndex &parent) const
{
  /*
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();


   return createIndex(row, column, m_transitions[row]);
  */
  // return m_turns.at(0)->index(row, column, parent);
   return currentDialogTurn->index(row, column, parent);
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
  // return m_turns.at(0)->data(index, role);
  return currentDialogTurn->data(index, role);
}

void DialogState::setCurrentTurn(int index)
{
  currentDialogTurn = m_turns[index];
}

int DialogState::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return 1;
}

void DialogState::setSilence(bool silence)
{
  currentDialogTurn->setSilence(silence);
}

void DialogState::setAnnounceRepeat(bool announce)
{
  currentDialogTurn->setAnnounceRepeat(announce);
}

DialogState::~DialogState()
{
  foreach (DialogTurn* t, m_turns)
    disconnect(t, SIGNAL(destroyed()), this, SLOT(turnDestroyed()));
  qDeleteAll(m_turns);
}

void DialogState::bindStateCommands(QList<Command*> commands)
{
  int stateId = SimonCommand::GreedyState + 1;
  foreach (DialogTurn *turn, m_turns)
  {
    QList<DialogCommand*> transitions = turn->getTransitions();

    foreach (DialogCommand* transition, transitions)
    {
      transition->createStateLink(stateId);
      commands << transition;
    }

    stateId++;
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

void DialogState::turnChanged()
{
  emit changed();
}

void DialogState::turnDestroyed()
{
  m_turns.removeAll(static_cast<DialogTurn*>(sender()));
}

