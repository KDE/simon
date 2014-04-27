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

#include "dialogfieldstore.h"
#include "dialogintegerfield.h"

DialogFieldStore::~DialogFieldStore()
{
  foreach(const QString& key, this->dialogFields.keys())
  {
    removeField(key);
  }
}

bool DialogFieldStore::removeField(const QString& name)
{
  if(!this->dialogFields.contains(name))
  {
    kWarning() << "Variable " << name << " does not exist.";
    return false;
  }
  delete this->dialogFields.take(name);
  return true;
}

bool DialogFieldStore::addField(const QString& type, const QString& name, const QString& value = QString())
{
    if(this->dialogFields.contains(name))
    {
	kWarning() << "Variable " << name << " already exists.";
	return false;
    }

    if(!this->creators.contains(type))
    {
	kWarning() << "Type " << type << " has no registered factories.";
	return false;
    }

    const DialogFieldTypeInfo* dfti = this->creators[type];
    DialogFieldBase* b = dfti->create(name,value);

    if(!b)
    {
      kWarning() << "Failed to create field " << name << " with value " << value;
      return false;
    }
    this->dialogFields[name] = b;
    return true;
}

bool DialogFieldStore::addField(DialogFieldBase* df)
{
  const QString& name = df->getName();

  if(this->dialogFields.contains(name))
  {
    kWarning() << "Variable " << name << " already exists.";
    return false;
  }

  Q_ASSERT(!df);

  this->dialogFields[name] = df;
  return true;
}

bool DialogFieldStore::registerDefaults()
{
  return this->registerFactory(DialogIntegerField::typeInfo);
}


bool DialogFieldStore::registerFactory(const DialogFieldTypeInfo& dfti)
{
  if(this->creators.contains(dfti.id))
  {
    kWarning() << "Factory method already registered for type " << dfti.id;
    return false;
  }
  this->creators[dfti.id] = &dfti;
  return true;
}

bool DialogFieldStore::unregisterFactory(const QString& key)
{
  if(!this->creators.contains(key))
  {
    kWarning() << "Type " << key << " is not registered.";
    return false;
  }
  this->creators.remove(key);
  return true;
}

bool DialogFieldStore::deSerialize(const QDomElement& elem)
{
   if(elem.isNull()) return false;
   QDomNodeList l = elem.elementsByTagName("field");
   for(unsigned i = 0;i<l.length();i++)
   {
      QDomElement e = l.at(i).toElement();
      QDomElement type = e.firstChildElement("type");
      if(type.isNull())
      {
	kWarning() << "Encountered an element without a type when deSerializing the DialogFieldStore";
	continue;
      }

      if(!this->creators.contains(type.text()))
      {
	kWarning() << "Type " << type.text() << " not registered in creators!";
	continue;
      }

      DialogFieldBase* result = this->creators[type.text()]->deSerialize(e);

      if(result)
      {
	this->dialogFields[result->getName()] = result;
      }
   }
   return true;
}

QDomElement DialogFieldStore::serialize(QDomDocument* doc)
{
  QDomElement root = doc->createElement("fieldstore");
  foreach(const QString& key, this->dialogFields.keys())
  {
    root.appendChild(this->dialogFields[key]->serialize(doc));
  }
  return root;
}
