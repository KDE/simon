/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "latexreporttemplateengine.h"
#include <QFileInfo>
#include <KLocalizedString>
#include <KZip>

QByteArray LatexReportTemplateEngine::lineBreak()
{
  return "\\\\\n";
}

QByteArray LatexReportTemplateEngine::escape(const QByteArray& in, bool extraSafety)
{
  QByteArray out = in;
  out = out.replace('%', "\\%");
  out = out.replace('#', "\\#");

  if (extraSafety)
    out = out.replace('_', '-');
  else
    out = out.replace('_', "\\_");

  return ReportTemplateEngine::escape(out, extraSafety);
}

bool LatexReportTemplateEngine::storeFile(const QByteArray& output, const QString& outputFilename, const QStringList& associatedFiles)
{
  KZip zip(outputFilename);
  if (!zip.open(QIODevice::WriteOnly)) return false;

  QFileInfo fi(outputFilename);
  zip.writeFile(fi.baseName()+".tex", "", "", output.constData(), output.size());
  foreach (const QString& file, associatedFiles)
  {
    QFileInfo assocFileInfo(file);
    QFile f(file);
    if (!f.open(QIODevice::ReadOnly)) continue;

    QByteArray data;
    while (!f.atEnd())
      data += f.read(4096);
    
    zip.writeFile(assocFileInfo.fileName(), "", "", data.constData(), data.size());
  }

  return zip.close();
}

QString LatexReportTemplateEngine::fileType()
{
  return i18n("LaTeX files (zipped) *.zip");
} 

