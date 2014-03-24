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

//TODO:  Implement the parser class and have this use it to get/set the variable.

class DialogVariableBase {
  public:
    DialogVariableBase() {}
    virtual ~DialogVariableBase() {}
    virtual QVariant getValue() = 0;
    virtual bool setValue(const QString& val) = 0;
};

template <class T>
class DialogVariable : public DialogVariableBase
{
  typedef T VariableType;
  private:
      QString name;
      VariableType value;
      class Parser {} parser;
  public:
      DialogVariable<T>(QString n, VariableType val) : name(n), value(val) { }
      VariableType getTypedValue() { return value; }
      virtual QVariant getValue()
      {
	  return QVariant(value);
      }
      virtual bool setValue(const QString& val)
      {
	Q_UNUSED(val);
	//TODO: Implement this with parsers
	return false;
      }
      virtual bool setTypedValue(const T& val)
      {
	value = val;
	return true;
      }
	
};