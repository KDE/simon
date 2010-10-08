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

#ifndef SIMON_REPORTTEMPLATEENGINE_H_4002119636CC42C68FE07273F9000A73
#define SIMON_REPORTTEMPLATEENGINE_H_4002119636CC42C68FE07273F9000A73

#include <QHash>

class ReportTemplateEngine
{
  protected:
    QString m_lastError;

    virtual QByteArray lineBreak();
    virtual QByteArray replaceTemplateParameters(const QByteArray& templateData, QHash<QString, QString> templateValues);

    QString tempDir();
    bool cleanTempDir();

    bool removeDir(const QString& path);

  public:
    virtual ~ReportTemplateEngine();
    QString lastError() { return m_lastError; }

    virtual bool parse(const QByteArray& templateData, QHash<QString, QString> templateValues, 
        bool useGraphs, bool useTables, const QString& outputFilename);

    virtual bool storeFile(const QByteArray& output, const QString& outputFilename);

    virtual QString fileType()=0;


};

#endif

