/*
 *  Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#include "reporttemplateengine.h"
#include "templatevaluelist.h"
#include <QDir>
#include <QFile>
#include <KLocale>
#include <KDebug>
#include <KStandardDirs>

ReportTemplateEngine::~ReportTemplateEngine()
{
}

QByteArray ReportTemplateEngine::lineBreak()
{
  return "\n";
}

QByteArray ReportTemplateEngine::replaceTemplateParameters(const QByteArray& templateData, QHash<QString, QString> templateValues)
{
  QHash<QString, QString>::const_iterator i = templateValues.constBegin();
  QHash<QString, QString>::const_iterator end = templateValues.constEnd();

  QByteArray output = templateData;

  while (i != end)
  {
    output = output.replace(QByteArray("$").append(i.key()).append("$"), i.value().toUtf8());
    i++;
  }
  return output;
}

bool ReportTemplateEngine::splitTemplate(const QByteArray& input, const QByteArray& id, 
    QByteArray& pre, QByteArray& part, QByteArray& post)
{
  QByteArray beginTag = "$BEGIN_"+id+"$";
  QByteArray endTag = "$END_"+id+"$";
  int startIndex = input.indexOf(beginTag);
  int endIndex = input.indexOf(endTag, startIndex);
  //kDebug() << "Looking for " << beginTag << endTag << " in " << input << startIndex << endIndex;
  if ((startIndex == -1) || (endIndex == -1))
    return false;
  pre = input.left(startIndex);
  part = input.mid(startIndex + beginTag.count(), endIndex-startIndex-beginTag.count());
  post = input.mid(endIndex+endTag.count());
  kDebug() << "Part: " << part;
  return true;
}

QByteArray ReportTemplateEngine::replaceTemplateLists(const QByteArray& templateData, QList<TemplateValueList*> templateValues)
{
  QByteArray output = templateData;

  int c = 0;
  foreach (TemplateValueList* value, templateValues)
  {
    QByteArray pre, part, post; 
    while ((c < 3) && (splitTemplate(output, value->id().toUtf8(), pre, part, post)))
    {
      output = pre;
      for (int i=0; i < value->count(); i++)
      {
        QHash<QString,QString> tValues = value->at(i);
        part = replaceTemplateParameters(part, tValues);
        output += part;
      }
      output += post;
      c++;
    }
  }
  return output;
}

QString ReportTemplateEngine::tempDir()
{
  return KStandardDirs::locateLocal("tmp", "sam/reports/tmp/");
}

bool ReportTemplateEngine::removeDir(const QString& path)
{
  QDir d(path);
  
  if (!d.exists(path)) return true;

  bool succ = true;
  QStringList files = d.entryList(QDir::Files|QDir::NoDotAndDotDot);
  foreach (const QString& f, files)
    succ = d.remove(f) && succ;

  QStringList subDirs = d.entryList(QDir::Dirs|QDir::NoDotAndDotDot);
  foreach (const QString& subDir, subDirs)
    succ = removeDir(subDir) && succ;

  return succ && d.rmdir(path);
}


bool ReportTemplateEngine::cleanTempDir()
{
  return removeDir(tempDir());
}

bool ReportTemplateEngine::parse(const QByteArray& templateData, QHash<QString, QString> templateValues, 
    QList<TemplateValueList*> templateValueLists,
    bool useGraphs, bool useTables, const QString& outputFilename)
{
  if (!cleanTempDir())
  {
    m_lastError = i18n("Couldn't clean temporary directory");
    return false;
  }

  QByteArray output;
  output = replaceTemplateParameters(templateData, templateValues);
  output = replaceTemplateLists(output, templateValueLists);

  if (useGraphs)
  {
    kDebug() << "Creating graphs...";
  }
  if (useTables)
  {
    kDebug() << "Creating tables...";
  }

  if (!storeFile(output, outputFilename))
  {
    m_lastError = i18n("Failed to store file.");
    return false;
  }
  return  true;
}

bool ReportTemplateEngine::storeFile(const QByteArray& output, const QString& outputFilename)
{
  QFile f(outputFilename);
  if (!f.open(QIODevice::WriteOnly))
    return false;
  f.write(output);
  f.close();
  return true;
}

