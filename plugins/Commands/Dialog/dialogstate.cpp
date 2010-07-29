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

#include "dialogstate.h"
#include "dialogcommand.h"
#include "dialogtext.h"
#include <QDomDocument>
#include <QDomElement>

DialogState::DialogState(DialogTextParser *parser, const QString& name, const QString& text, 
    QList<DialogCommand*> transitions, QObject *parent) : 
  QAbstractItemModel(parent),
  m_name(name),
  m_text(new DialogText(parser, text)),
  m_transitions(transitions)
{
}

DialogState* DialogState::createInstance(DialogTextParser *parser, const QDomElement& elem)
{
  DialogState *state = new DialogState();

  if (!state->deSerialize(parser, elem))
  {
    delete state;
    return NULL;
  }

  return state;
}

QString DialogState::getText() const
{
  return m_text->parse();
}

bool DialogState::deSerialize(DialogTextParser *parser, const QDomElement& elem)
{
  if (elem.isNull()) return false;

  m_name = elem.attribute("name");

  QDomElement text = elem.firstChildElement("text");

  QDomElement transitions = elem.firstChildElement("transitions");
  QDomElement transition = transitions.firstChildElement("command");

  QList<DialogCommand*> commands;
  kDebug() << "Deserializing transitions";
  while (!transition.isNull())
  {
    kDebug() << "Deserializing transition!";
    DialogCommand *c = DialogCommand::createInstance(transition);
    if (c)
    {
      connect(c, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
      commands << c;
    }

    transition = transition.nextSiblingElement("command");
  }

  m_text = new DialogText(parser, text.text());
  m_transitions = commands;
  kDebug() << "Deserialized state: " << text.text();
  return true;
}

QDomElement DialogState::serialize(QDomDocument *doc)
{
  QDomElement elem = doc->createElement("state");
  elem.setAttribute("name", m_name);

  QDomElement textElem = doc->createElement("text");
  textElem.appendChild(doc->createTextNode(m_text->source()));
  QDomElement transitionsElem = doc->createElement("transitions");
  
  foreach (DialogCommand *c, m_transitions)
    transitionsElem.appendChild(c->serialize(doc));

  elem.appendChild(textElem);
  elem.appendChild(transitionsElem);

  return elem;
}

void DialogState::addTransition(DialogCommand* command)
{
  beginInsertRows(QModelIndex(), m_transitions.count(), m_transitions.count());

  m_transitions << command;
  connect(command, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
  emit changed();

  endInsertRows();
}

void DialogState::removeTransition(DialogCommand* command)
{
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
}


Qt::ItemFlags DialogState::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DialogState::headerData(int column, Qt::Orientation orientation,
                  int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18n("Name");
    }
  }

  //default
  return QVariant();
}

QModelIndex DialogState::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}

int DialogState::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return m_transitions.count();
}


QModelIndex DialogState::index(int row, int column,const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();


   return createIndex(row, column, m_transitions[row]);
}

QVariant DialogState::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole)
    return m_transitions.at(index.row())->getTrigger();

  if (role == Qt::DecorationRole)
    return m_transitions.at(index.row())->getIcon();

  return QVariant();
}

int DialogState::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return 1;
}

DialogState::~DialogState()
{
  delete m_text;
}

