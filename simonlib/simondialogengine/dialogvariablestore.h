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

//Ben Notes:  An instance of this class is essentially going to be a list
//            of active variables for the current Dialog Instance.
//TODO: Add factory methods for generating DialogVariable.

class DialogVariableBase;
class DialogStringVariable;
class DialogIntegerVariable;
class DialogDecimalVariable;
template <class T>
class DialogVariable;

class DialogVariableStore {
  private:
    QMap<QString,DialogVariableBase*> dialogVariables;
    class DialogVariableFactory
    {
      public:
	DialogVariable<QString> * CreateString(QString n, QString val) { return Create<QString>(n,val); }
	DialogVariable<int> * CreateInteger(QString n, int val) { return Create<int>(n,val); }
	DialogVariable<double> * CreateDecimal(QString n, double val) { return Create<double>(n,val); }
	template <typename T>
	DialogVariable<T> * Create(QString n, T val) {return new DialogVariable<T>(n,val);};
    } factory;
  public:
    void removeVariable(const QString& name);
    QString getValue(const QString& name) const;
    int count() { return dialogVariables.count(); }
    
    template <typename T>
    void addVariable(const QString& name, const T& val)
    {
      Q_ASSERT(!this->dialogVariables.contains(name));
      this->dialogVariables[name] = this->factory.Create<T>(name,val);
    }
    
    template <typename T>
    T getTypedValue(const QString& name) const
    {
      //TODO:  Fix this so it doesn't return if there's a bad cast.
      if(DialogVariable<T> * temp = dynamic_cast<DialogVariable<T>*>(this->dialogVariables[name]))
      {
	return temp->getTypedValue();
      } 
      return T();
    }
    
    
    template <typename T>
    void setVariable(const QString& name, const T& value)
    {
      removeVariable(name);
      addVariable<T>(name,value);
    }
};
