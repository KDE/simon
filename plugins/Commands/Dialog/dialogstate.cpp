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
    bool silence, bool announceRepeat,
    QList<DialogCommand*> transitions, QObject *parent) : 
  QAbstractItemModel(parent),
  m_name(name),
  m_text(new DialogText(parser, text)),
  m_silence(silence),
  m_announceRepeat(announceRepeat),
  m_transitions(transitions)
{
  foreach (DialogCommand *c, m_transitions)
  {
    connect(c, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
    connect(c, SIGNAL(changed()), this, SIGNAL(changed()));
  }
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

QString DialogState::getRawText() const
{
  return m_text->source();
}

bool DialogState::setRawText(const QString& data)
{
  m_text->setSource(data);
  emit changed();
  return true;
}

void DialogState::presented()
{
  foreach (DialogCommand *c, m_transitions)
    c->presented();
}

void DialogState::left()
{
  foreach (DialogCommand *c, m_transitions)
    c->left();
}

bool DialogState::deSerialize(DialogTextParser *parser, const QDomElement& elem)
{
  if (elem.isNull()) return false;

  m_name = elem.attribute("name");

  QDomElement text = elem.firstChildElement("text");
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

  m_text = new DialogText(parser, text.text());
  m_transitions = commands;
  return true;
}

QDomElement DialogState::serialize(QDomDocument *doc)
{
  QDomElement elem = doc->createElement("state");
  elem.setAttribute("name", m_name);

  QDomElement textElem = doc->createElement("text");
  textElem.appendChild(doc->createTextNode(m_text->source()));
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

  elem.appendChild(textElem);
  elem.appendChild(textOptions);
  elem.appendChild(avatarElem);
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

bool DialogState::moveTransitionUp(DialogCommand* command)
{
  int i = m_transitions.indexOf(command);
  if (i <= 0) return false;

  m_transitions.insert(i-1, m_transitions.takeAt(i));
  emit changed();
  emit dataChanged(index(i-1, 0),index(i, 0));
  return true;
}

bool DialogState::moveTransitionDown(DialogCommand* command)
{
  int i = m_transitions.indexOf(command);
  if ((i == -1) || (i == (m_transitions.count()-1))) 
    return false;

  m_transitions.insert(i+1, m_transitions.takeAt(i));
  emit changed();
  emit dataChanged(index(i, 0),index(i+1, 0));
  return true;
}

bool DialogState::rename(const QString& name)
{
  m_name = name;
  emit changed();
  return true;
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
        return i18nc("Name of the state", "Name");
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

void DialogState::setSilence(bool silence)
{
  m_silence = silence;
}

void DialogState::setAnnounceRepeat(bool announce)
{
  m_announceRepeat = announce;
}


DialogState::~DialogState()
{
  delete m_text;
}

