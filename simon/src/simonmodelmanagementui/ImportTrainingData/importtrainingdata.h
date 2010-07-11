/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_IMPORTTRAININGDATA_H_8910EF5AC0954D49A08BD1AA80C41BEA
#define SIMON_IMPORTTRAININGDATA_H_8910EF5AC0954D49A08BD1AA80C41BEA

#include <QThread>

class PostProcessing;
/**
  @author Peter Grasch <bedahr@gmx.net>
*/
class ImportTrainingData : public QThread
{
  Q_OBJECT
    signals:
  void progress(int now, int max=-1);
  void done();
  void error(QString);
  void status(QString);
  private:
    PostProcessing *pp;
    QString directory, promptsPath, basePath;
    bool isPrompts;
    int prog;

    QString extractSaid(QString source);
    QStringList* processSounds(QStringList files, QString destDir);
    bool createPrompts(QStringList dataFiles);
    QStringList* searchDir(QString dir);

  public slots:
    void run();

  public:
    ImportTrainingData(QObject* parent);

    bool import(bool isPrompts, QString path, QString basePath);

    ~ImportTrainingData();

};
#endif
