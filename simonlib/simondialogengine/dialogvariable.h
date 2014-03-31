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

#include <QString>
#include <QVariant>
#include <QSharedPointer>

//TODO:  Implement the parser class and have this use it to get/set the variable.


template <class T>
class DialogVariable;

template <class T>
class DialogVariableValue
{
  private:
    QSharedPointer<T> ptr;
    bool isValidCast;
  public:
    DialogVariableValue<T>() : ptr(), isValidCast(false) { }
    DialogVariableValue<T>(const DialogVariableValue<T>& cpy) : ptr(cpy.ptr), isValidCast(cpy.isValidCast) { }
    explicit DialogVariableValue<T>(const QSharedPointer<T>& pointer) : ptr(pointer), isValidCast(true) { }
    
    T* data() { return this->ptr.data(); }
    
    bool isValid() { return this->isValidCast; }
    
    /*DialogVariableValue<T>& operator=(const T& val) 
    { 
      *ptr = val;
      return *this;
    }*/
};

class DialogVariableBase {
  protected:
    //TODO: Find a less hacky solution to this.
  public:
    DialogVariableBase() {}
    virtual ~DialogVariableBase() {}
    //virtual QVariant getValue() = 0;
    virtual bool setValue(const QString& val) = 0;
    virtual QString toString() = 0;
    
    
    template <class T>
    DialogVariableValue<T> getValue()
    {
      if(DialogVariable<T> * casted_this = dynamic_cast<DialogVariable<T>*>(this))
      {
	return casted_this->getVal();
      }
      else
      {
	return DialogVariableValue<T>();
      }
    }
};


template <class T>
class DialogVariable : public DialogVariableBase
{
  typedef T VariableType;
  private:
      QString name;
      QSharedPointer<VariableType> value;
      class Parser {} parser;
  public:
      DialogVariable<T>(const QString& n, const VariableType& val) : name(n), value(new VariableType(val)) { }
      
      DialogVariableValue<T> getVal()
      {
	return DialogVariableValue<T>(value);
      }
      
      virtual bool setValue(const QString& val)
      {
	Q_UNUSED(val);
	//TODO: Implement this with parsers
	return false;
      }
      
      virtual QString toString() { return "test"; }
};