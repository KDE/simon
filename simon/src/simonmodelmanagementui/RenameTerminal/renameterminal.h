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

#ifndef SIMON_RENAMETERMINAL_H_953886639A0B4A90BE6F5E94B338072A
#define SIMON_RENAMETERMINAL_H_953886639A0B4A90BE6F5E94B338072A

#include <QThread>

/**
  @author Peter Grasch <bedahr@gmx.net>
*/
class RenameTerminal : public QThread
{
  Q_OBJECT
    signals:
  void progress(int);
  void done();
  private:
    QString oldName, newName;
    bool includeShadow, includeGrammar;
  public:
    RenameTerminal(QObject* parent);
    void setOldName(QString old) { oldName = old; }
    void setNewName(QString newName) { this->newName = newName; }
    void setIncludeShadow (bool includeShadow)
      { this->includeShadow = includeShadow; }
    void setIncludeGrammar (bool includeGrammar)
      { this->includeGrammar = includeGrammar; }
    ~RenameTerminal();

    void run();

};
#endif
