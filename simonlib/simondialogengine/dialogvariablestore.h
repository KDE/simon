/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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
#include <QList>
#include <QString>

//Ben Notes:  An instance of this class is essentially going to be a list
//            of active variables for the current Dialog Instance.

class DialogVariable {};

class DialogVariableStore {
  private:
    QMap<QString,DialogVariable*> dialogVariables;
  public:
    void addVariable(QString& name, DialogVariable* dialogVariable);
    int removeVariable(QString& name);
    const DialogVariable* get(QString& name) const;
    int count() { return dialogVariables.count(); }
};
