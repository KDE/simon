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

#include "dialogturn.h"
#include "dialogcommand.h"
#include "dialogtext.h"
#include <QDomDocument>
#include <QDomElement>
#include <QDateTime>

DialogTurn::DialogTurn(DialogTextParser *parser, const QString& name, const QString& text, 
    bool silence, bool announceRepeat,
    QList<DialogCommand*> transitions, QObject *parent) : 
  QAbstractItemModel(parent),
  m_name(name),
  m_currentRandomTextIndex(0), /*only one text*/
  m_parser(parser),
  m_silence(silence),
  m_announceRepeat(announceRepeat),
  m_transitions(transitions)
{
  qsrand(QDateTime::currentDateTime().toTime_t());
  m_texts << new DialogText(parser, text);
  foreach (DialogCommand *c, m_transitions)
  {
    kDebug() << "Registering dialog command: " << c->getTrigger();
    connect(c, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
    connect(c, SIGNAL(changed()), this, SIGNAL(changed()));
  }
}

DialogTurn* DialogTurn::createInstance(DialogTextParser *parser, const QDomElement& elem)
{
  DialogTurn *turn = new DialogTurn();

  if (!turn->deSerialize(parser, elem))
  {
    delete turn;
    return NULL;
  }

  return turn;
}

int DialogTurn::getTextCount()
{
  return m_texts.count();
}

int DialogTurn::addText(const QString& text)
{
  m_texts << new DialogText(m_parser, text);
  updateRandomTextSelection();
  return m_texts.count() - 1;
}

bool DialogTurn::removeText(int id)
{
  if (id >= m_texts.count())
    return false;
  
  delete m_texts.takeAt(id);
  updateRandomTextSelection();
  return true;
}

void DialogTurn::updateRandomTextSelection()
{
  //yeah, yeah non-even distribution and predictable randoms on old implementations..
  //who cares for this purpose :)
  m_currentRandomTextIndex = qrand() % m_texts.count();
}

QString DialogTurn::getText() const
{
  return m_texts[m_currentRandomTextIndex]->parse();
}

QString DialogTurn::getRawText(int index) const
{
  return m_texts[index]->source();
}

bool DialogTurn::setRawText(int index, const QString& data)
{
  m_texts[index]->setSource(data);
  emit changed();
  return true;
}

void DialogTurn::presented()
{
  foreach (DialogCommand *c, m_transitions)
    c->presented();
}

void DialogTurn::left()
{
  foreach (DialogCommand *c, m_transitions)
    c->left();
}

bool DialogTurn::deSerialize(DialogTextParser *parser, const QDomElement& elem)
{
  if (elem.isNull()) return false;

  m_name = elem.attribute("name");
  m_parser = parser;

  qDeleteAll(m_texts);
  m_texts.clear();
  
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
}

QDomElement DialogTurn::serialize(QDomDocument *doc)
{
  QDomElement elem = doc->createElement("turn");
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
}

void DialogTurn::addTransition(DialogCommand* command)
{
  beginInsertRows(QModelIndex(), m_transitions.count(), m_transitions.count());

  m_transitions << command;
  connect(command, SIGNAL(requestDialogState(int)), this, SIGNAL(requestDialogState(int)));
  emit changed();

  endInsertRows();
}

void DialogTurn::removeTransition(DialogCommand* command)
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

bool DialogTurn::moveTransitionUp(DialogCommand* command)
{
  int i = m_transitions.indexOf(command);
  if (i <= 0) return false;

  m_transitions.insert(i-1, m_transitions.takeAt(i));
  emit changed();
  emit dataChanged(index(i-1, 0),index(i, 0));
  return true;
}

bool DialogTurn::moveTransitionDown(DialogCommand* command)
{
  int i = m_transitions.indexOf(command);
  if ((i == -1) || (i == (m_transitions.count()-1))) 
    return false;

  m_transitions.insert(i+1, m_transitions.takeAt(i));
  emit changed();
  emit dataChanged(index(i, 0),index(i+1, 0));
  return true;
}

bool DialogTurn::rename(const QString& name)
{
  m_name = name;
  emit changed();
  return true;
}


Qt::ItemFlags DialogTurn::flags(const QModelIndex &index) const
{
  if (!index.isValid())
    return 0;

  return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
}

QVariant DialogTurn::headerData(int column, Qt::Orientation orientation,
                  int role) const
{
  if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
    switch (column) {
      case 0:
        return i18nc("Name of the turn", "Name");
    }
  }

  //default
  return QVariant();
}

QModelIndex DialogTurn::parent(const QModelIndex &index) const
{
  Q_UNUSED(index);
  return QModelIndex();
}

int DialogTurn::rowCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent);
  return m_transitions.count();
}


QModelIndex DialogTurn::index(int row, int column,const QModelIndex &parent) const
{
  if (!hasIndex(row, column, parent) || parent.isValid())
    return QModelIndex();


   return createIndex(row, column, m_transitions[row]);
}

QVariant DialogTurn::data(const QModelIndex &index, int role) const
{
  if (!index.isValid()) return QVariant();

  if (role == Qt::DisplayRole)
    return m_transitions.at(index.row())->getTrigger();

  if (role == Qt::DecorationRole)
    return m_transitions.at(index.row())->getIcon();

  return QVariant();
}

int DialogTurn::columnCount(const QModelIndex &parent) const
{
  Q_UNUSED(parent)
  return 1;
}

void DialogTurn::setSilence(bool silence)
{
  m_silence = silence;
}

void DialogTurn::setAnnounceRepeat(bool announce)
{
  m_announceRepeat = announce;
}


DialogTurn::~DialogTurn()
{
  qDeleteAll(m_texts);
}

