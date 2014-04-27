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

#ifndef SIMON_DIALOGFIELD_H_30683738694b47f68743788cf61493e6
#define SIMON_DIALOGFIELD_H_30683738694b47f68743788cf61493e6

#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QSharedPointer>
#include <KDebug>

//TODO:  Implement the parser class and have this use it to get/set the variable.
//TODO:  Split the DialogFieldValue and DialogFieldInfo classes into their own separate files.

template <class T>
class DialogField;
class DialogFieldBase;

class QDomElement;
class QDomDocument;

class DialogFieldCreator
{

};

class DialogFieldTypeInfo
{
  public:
    typedef DialogFieldBase* (*deSerializeFunction)(const QDomElement& elem);
    typedef DialogFieldBase* (*createFunction)(const QString& name, const QString& value);

    const QString id;
    const QString name;
    const QString description;

    const deSerializeFunction deSerialize;
    const createFunction create;

    DialogFieldTypeInfo(const QString id_, const QString name_, const QString desc_, const deSerializeFunction dfs_ptr,
			const createFunction cf_ptr) : id(id_), name(name_), description(desc_),
							deSerialize(dfs_ptr), create(cf_ptr) { }
};

template <class T>
class DialogFieldValue
{
  private:
    QSharedPointer<T> ptr;
    bool isValidCast;
  public:
    DialogFieldValue<T>() : ptr(), isValidCast(false) { }
    explicit DialogFieldValue<T>(const QSharedPointer<T>& pointer) : ptr(pointer), isValidCast(!pointer.isNull()) { }

    T* data() { return this->ptr.data(); }

    bool isValid() { return this->isValidCast; }
};

class DialogFieldBase {
  protected:
    virtual const QString& getType() const = 0;
  public:
    DialogFieldBase() {}
    virtual ~DialogFieldBase() {}
    virtual QString toString() = 0;

    virtual bool deSerialize(const QDomElement& elem) = 0;
    virtual QDomElement serialize(QDomDocument * doc) = 0;

    virtual QString getName() const = 0;

    template <class T>
    DialogFieldValue<T> getValue()
    {
      if(DialogField<T> * casted_this = dynamic_cast<DialogField<T>*>(this))
      {
	return casted_this->getVal();
      }
      else
      {
	kWarning() << "Get value cast failed!";
	return DialogFieldValue<T>();
      }
    }
};

template <class T>
class DialogField : public DialogFieldBase
{
  protected:
    typedef T VariableType;

    QString name;
    QSharedPointer<VariableType> value;

    DialogField<T>() : name(), value() { }
    DialogField<T>(const QString& n) : name(n), value() { }
    virtual const QString& getType() const = 0;

    virtual QSharedPointer<VariableType> parseValue(const QString& value) = 0;

    virtual QSharedPointer<VariableType> deSerializeValue(const QDomElement& elem) = 0;
    virtual QDomElement seriaizeValue(QDomDocument * doc) = 0;
  private:
      class Parser {} parser;
  public:
      DialogField<T>(const QString& n, const VariableType& val) : name(n), value(new VariableType(val)) { }
      DialogField<T>(const QString& n, const QSharedPointer<VariableType>& val) : name(n), value(val) { }
      DialogFieldValue<T> getVal() const
      {
	return DialogFieldValue<T>(value);
      }

      virtual QString getName() const
      {
	return name;
      }

      virtual QDomElement serialize(QDomDocument * doc)
      {
	Q_ASSERT(doc);

	QDomElement elem = doc->createElement("field");

	QDomElement name = doc->createElement("name");
	QDomElement value = this->seriaizeValue(doc);
	QDomElement type = doc->createElement("type");

	QDomText name_value = doc->createTextNode(this->name);
	name.appendChild(name_value);

	QDomText type_value = doc->createTextNode(this->getType());
	type.appendChild(type_value);

	elem.appendChild(name);
	elem.appendChild(type);
	elem.appendChild(value);

	return elem;
      }

      virtual bool deSerialize(const QDomElement& elem)
      {
	//Assumption; elem is a "field" elem
	if(elem.isNull()) return false;
	if(elem.firstChildElement("type").text() != getType()) return false;

	this->name = elem.firstChildElement("name").text();
	this->value = this->deSerializeValue(elem.firstChildElement("value"));

	if(this->value.isNull())
	{
	  return false;
	}

	return true;
      }

      virtual QString toString() = 0;
};

#endif /* SIMON_DIALOGFIELD_H_30683738694b47f68743788cf61493e6 */
