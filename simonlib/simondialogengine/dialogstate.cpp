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

#include <iostream>

DialogState::DialogState(DialogTextParser* parser, const QString& name, QList<DialogTurn*> turns, QObject* parent) :
  QAbstractItemModel(parent),
  m_name(name),
  m_currentRandomTurnIndex(0), /* only one turn */
  m_parser(parser),
  m_turns(turns),
  currentDialogTurn(0)
{
  qsrand(QDateTime::currentDateTime().toTime_t());
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
  //I don't think the UI needs to get updated of turnChanged events because
  //the UI doesn't show anything relating to the turn besides the name.
  m_turns << turn;
  currentDialogTurn = turn;
}

bool DialogState::removeTurn()
{
  DialogTurn* turn = getCurrentTurn();
  if (!(turn && m_turns.size()))
    return false;
  m_turns.removeAll(turn);
  currentDialogTurn = 0;

  delete turn;
  return true;
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
  return currentDialogTurn->getTextCount();
}

int DialogState::addText(const QString& text)
{
  return currentDialogTurn->addText(text);
}

bool DialogState::removeText(int id)
{
  return currentDialogTurn->removeText(id);
}

void DialogState::updateRandomTextSelection()
{
  m_currentRandomTurnIndex = qrand() % m_turns.count();
  currentDialogTurn = m_turns[m_currentRandomTurnIndex];
  currentDialogTurn->updateRandomTextSelection();
}

QString DialogState::getText() const
{
  return m_turns[m_currentRandomTurnIndex]->getText();
}

QString DialogState::getRawText(int index) const
{
  return currentDialogTurn->getRawText(index);
}

bool DialogState::setRawText(int index, const QString& data)
{
  return currentDialogTurn->setRawText(index, data);
}

void DialogState::presented()
{
  currentDialogTurn->presented();
}

void DialogState::left()
{
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
    }
    else
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
  currentDialogTurn->addTransition(command);
}

void DialogState::removeTransition(DialogCommand* command)
{
  currentDialogTurn->removeTransition(command);
}

bool DialogState::moveTransitionUp(DialogCommand* command)
{
  return currentDialogTurn->moveTransitionUp(command);
}

bool DialogState::moveTransitionDown(DialogCommand* command)
{
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
  return currentDialogTurn->flags(index);
}

QVariant DialogState::headerData(int column, Qt::Orientation orientation,
                  int role) const
{
  return currentDialogTurn->headerData(column, orientation, role);
}

QModelIndex DialogState::parent(const QModelIndex &index) const
{
  return currentDialogTurn->parent(index);
}

int DialogState::rowCount(const QModelIndex &parent) const
{
  return currentDialogTurn->rowCount(parent);
}


QModelIndex DialogState::index(int row, int column, const QModelIndex &parent) const
{
   return currentDialogTurn->index(row, column, parent);
}

QVariant DialogState::data(const QModelIndex &index, int role) const
{
  return currentDialogTurn->data(index, role);
}

void DialogState::setCurrentTurn(int index)
{
  if (index < 0)
  {
    currentDialogTurn = 0;
  }
  else
  {
    currentDialogTurn = m_turns[index];
  }
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

bool DialogState::silence() const
{
  return m_turns.at(0)->silence();
}

bool DialogState::announceRepeat() const
{
  return m_turns.at(0)->announceRepeat();
}

bool DialogState::getDisplayAvatar() const
{
  return m_turns.at(0)->getDisplayAvatar();
}

void DialogState::setDisplayAvatar(bool display)
{
  m_turns.at(0)->setDisplayAvatar(display);
}

int DialogState::getAvatarId() const
{
  return m_turns.at(0)->getAvatarId();
}

void DialogState::setAvatar(int id)
{
  m_turns.at(0)->setAvatar(id);
}

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
