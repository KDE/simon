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

DialogState::DialogState(DialogTextParser *parser, const QString& text, QList<DialogCommand*> transitions,
    QObject *parent) : 
  QObject(parent),
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
  QDomElement textElem = doc->createElement("text");
  textElem.appendChild(doc->createTextNode(m_text->source()));
  QDomElement transitionsElem = doc->createElement("transitions");
  
  foreach (DialogCommand *c, m_transitions)
    transitionsElem.appendChild(c->serialize(doc));

  elem.appendChild(textElem);
  elem.appendChild(transitionsElem);

  return elem;
}

DialogState::~DialogState()
{
  delete m_text;
}

