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

#ifndef SIMON_REPORTTEMPLATEENGINE_H_4002119636CC42C68FE07273F9000A73
#define SIMON_REPORTTEMPLATEENGINE_H_4002119636CC42C68FE07273F9000A73

#include <QHash>
#include <QList>
#include <QString>
class TemplateValueList;

class ReportTemplateEngine
{
  protected:
    QString m_lastError;

    virtual QByteArray lineBreak();
    virtual QByteArray escape(const QByteArray& in, bool extraSafety);
    virtual QByteArray replaceTemplateParameters(const QByteArray& templateData, QHash<QString, QString> templateValues);
    virtual QByteArray replaceTemplateLists(const QByteArray& templateData, QList<TemplateValueList*> templateValues);
    virtual QByteArray replaceTemplateList(const QByteArray& templateData, TemplateValueList* templateValues);
    virtual QByteArray parseIf(const QByteArray& templateData, const QString& condition, bool value);

    virtual bool splitTemplate(const QByteArray& input, const QByteArray& id, const QByteArray& conditionStartPrefix,
        const QByteArray& conditionEndPrefix, QByteArray& pre, QByteArray& part, QByteArray& post);

    virtual QByteArray createGraphs(const QByteArray& input, const QList< TemplateValueList* >& options, QStringList& associatedFiles);

    QString tempDir();
    bool cleanTempDir();

    bool removeDir(const QString& path);
    
    float parsePrettyPrintedPercentage(QString in);

  public:
    virtual ~ReportTemplateEngine();
    QString lastError() { return m_lastError; }

    virtual bool parse(const QByteArray& templateData, QHash<QString, QString> templateValues, 
        QList<TemplateValueList*> templateValueLists,
        bool useGraphs, bool useTables, const QString& outputFilename);

    virtual bool storeFile(const QByteArray& output, const QString& outputFilename, const QStringList& associatedFiles);

    virtual QString fileType()=0;


};

#endif

