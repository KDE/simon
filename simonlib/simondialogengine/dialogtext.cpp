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

#include "dialogtext.h"
#include "dialogtextparser.h"
#include <KDebug>


DialogText::DialogText(DialogTextParser *textParser, const QString& data) :
  m_textParser(textParser),
  m_data(data)
{
}

QString DialogText::parse() const
{
  kDebug() << "parsing text: " << m_data;
  QString out = m_data;
  
  bool ok = m_textParser->parse(out);

  if (!ok)
    kWarning() << "Failed to parse: " << out;

  out = QString("<html><head /><body><p>%1</p></body></html>").arg(out.replace('\n', "<br />"));

  return out;
}

DialogText::~DialogText()
{
}

