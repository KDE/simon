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

#include "dialogtextparser.h"
#include "dialogtemplateoptions.h"
#include "dialogboundvalues.h"

#include <QStringList>
#include <QRegExp>
#include <QVariant>

#include <KDebug>

DialogTextParser::DialogTextParser(DialogTemplateOptions* templateOptions, DialogBoundValues* boundValues) :
  m_templateOptions(templateOptions),
  m_boundValues(boundValues)
{
}

void DialogTextParser::setArguments(const QStringList& arguments)
{
  m_boundValues->setArguments(arguments);
}


bool DialogTextParser::parseTemplates(QString& data)
{
  QString outData;

  //find {{x}}
  //"{{bla}}gu{{endbla}}dudu"
  
  int startPos=-1;
  int oldStartPos=0;
  int endPos=data.count();
  QString condition=QString();

  QStringList activeLimitingConditions;
  QStringList activeMetConditions;

  forever
  {
    oldStartPos = startPos;
    startPos = data.indexOf("{{", oldStartPos+1);

    //output
    //only output if every condition is met
    if (activeLimitingConditions.isEmpty())
    {
      if (startPos == -1) 
      {
        if (oldStartPos == -1)
          outData += data.mid(oldStartPos);
        else
          outData += data.mid(oldStartPos+4+condition.count());

        break;
      } else {
        if (oldStartPos == -1)
          outData += data.mid(0, startPos);
        else
          outData += data.mid(oldStartPos+4+condition.count(), startPos-oldStartPos-4-condition.count());
      }
    } else {
      if (startPos == -1)
      {
        kWarning() << "Syntax error: Unclosed conditions: " << activeLimitingConditions;
        return false;
      }
    }
    //endoutput

    endPos = data.indexOf("}}", (startPos == -1) ? 0 : startPos);
    if (endPos == -1)
    {
      kWarning() << "Syntax error: Condition not closed (missing }}) at position " << startPos;
      return false;
    }

    condition = data.mid(startPos+2, endPos-startPos-2);
    kDebug() << "Found condition: " << condition;

    if (!condition.startsWith("end"))
    {
      if (condition.startsWith("else"))
      {
        QString baseCondition = condition.mid(4);
        kDebug() << "Parsing else for " << baseCondition;
        bool wasActive = (activeLimitingConditions.removeAll(baseCondition) != 0);
        if (wasActive) activeMetConditions << baseCondition;
        else {
          if (activeMetConditions.removeAll(baseCondition) == 0)
          {
            kWarning() << "Else for unopened condition: " << baseCondition;
            return false;
          }
          activeLimitingConditions << baseCondition;
        }
      } else {
        //condition only relevant if it is NOT enabled in the template options
        if (!m_templateOptions->isEnabled(condition))
        {
          kDebug() << "Is not enabled: " << condition;
          activeLimitingConditions << condition;
        } else {
          kDebug() << "Is enabled: " << condition;
          activeMetConditions << condition;
        }
      }
    } else {
      QString baseCondition = condition.mid(3);
      if ((activeLimitingConditions.removeAll(baseCondition) == 0) &&
          (activeMetConditions.removeAll(baseCondition) == 0))
      {
        kWarning() << "Closed unopened condition " << baseCondition << " at position " << startPos;
        return false;
      }
    }
  }

  data = outData;

  return true;
}

bool DialogTextParser::parseBoundValues(QString& data)
{
  QString outData = data;

  //TODO: processing

  int beginPos = outData.indexOf("$");
  int endPos = -1;

  kDebug() << "Processing";

  while (beginPos != -1)
  {
    kDebug() << "Found variable";
    endPos = outData.indexOf("$", beginPos + 1);
    if (endPos == -1)
    {
      kWarning() << "Syntax error: Unclosed variable at " << beginPos;
      return false;
    }
    QString variable = outData.mid(beginPos+1, endPos-beginPos-1);
    kDebug() << "Requesting variable" << variable;

    QVariant value = m_boundValues->getBoundValue(variable);
    if (value.isNull())
    {
      kWarning() << "Variable not bound: " << variable;
      return false;
    }
    
    outData = outData.replace(beginPos, endPos-beginPos+1, value.toString());
    beginPos = outData.indexOf("$", beginPos+1);
  }

  data = outData;
  return true;
}

bool DialogTextParser::parse(QString& data)
{
  kDebug() << "Parsing: " << data;
  bool ok = parseTemplates(data);

  if (!ok) return false;

  ok = parseBoundValues(data);

  return ok;
}

DialogTextParser::~DialogTextParser()
{
}


