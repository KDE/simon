/*
 *   Copyright (C) 2014 Benjamin Bowley-Bryant <benbb@utexas.edu>
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
#include <QHash>
#include "dialogvariable.h"
#include <KDebug>

//Ben Notes:  An instance of this class is essentially going to be a list
//            of active variables for the current Dialog Instance.
//TODO: Add factory methods for generating DialogVariable.

class DialogVariableStore {
  private:
    QHash<QString,DialogVariableBase*> dialogVariables;
    QHash<QString,DialogFieldTypeInfo const *> creators;
    //TODO: Remove depricated DVF
    /*class DialogVariableFactory
    {
      public:
	template <typename T>
	DialogVariable<T> * Create(QString n, T val) {return new DialogVariable<T>(n,val);};
    } factory;*/
  public:
    bool removeVariable(const QString& name);
    int count() const { return dialogVariables.count(); }
    bool contains(QString s) { return dialogVariables.contains(s); }
    bool registerFactory(const QString& key, const DialogFieldTypeInfo& dfti);
  /*
    template <typename T>
    bool addVariable(const QString& name, const T& val)
    {
      if(this->dialogVariables.contains(name))
      {
	 kWarning() << "Variable " << name << " already exists.";
	 return false;
      }
      this->dialogVariables[name] = this->factory.Create<T>(name,val);
      return true;
    }
  */

    template <typename T>
    DialogVariableValue<T> getValue(const QString& name) const
    {
	return this->dialogVariables[name]->getValue<T>();
    }
};
