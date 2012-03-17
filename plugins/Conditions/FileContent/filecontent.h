/*
 *   Copyright (C) 2011 Peter Grasch <grasch@simon-listens.org>
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef FILECONTENT_H
#define FILECONTENT_H

/** \file filecontent.h
 * \brief The file containing the FileContent baseclass header.
 */

#include "simoncontextdetection/condition.h"
#include "simoncontextdetection/simoncontextdetection_export.h"
#include "simoncontextdetection/compoundcondition.h"
#include <QRegExp>

class QFileSystemWatcher;

class FileContent : public Condition
{
    Q_OBJECT
public:
    explicit FileContent(QObject *parent, const QVariantList& args);
    virtual CreateConditionWidget* getCreateConditionWidget(CompoundCondition *compoundCondition, QWidget *parent);

    QString getFilename() {return m_filename;}
    QString getFileContent() {return m_fileContent;}

    bool getFileContentIsRegExp() {return m_fileContentIsRegularExpression;}

    virtual QString name();


private:
    bool privateDeSerialize(QDomElement elem);
    QDomElement privateSerialize(QDomDocument *doc, QDomElement elem);

    QString m_filename;
    QString m_fileContent;

    bool m_fileContentIsRegularExpression;
    QRegExp m_fileContentRegExp;
    QFileSystemWatcher *watcher;

private slots:
    void checkFile();
    void checkFileInternal();

};

#endif // FILECONTENT_H
