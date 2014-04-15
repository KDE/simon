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

bool DialogFieldStore::removeVariable(const QString& name)
{
  if(!this->dialogVariables.contains(name))
  {
    kWarning() << "Variable " << name << " does not exist.";
    return false;
  }
  delete this->dialogVariables.take(name);
  return true;
}

bool DialogFieldStore::registerFactory(const QString& key, const DialogFieldTypeInfo& dfti)
{
  if(this->creators.contains(key))
  {
    kWarning() << "Factory method already registered for key " << key;
    return false;
  }
  this->creators[key] = &dfti;
  return true;
}

bool DialogFieldStore::unregisterFactory(const QString& key)
{
  if(!this->creators.contains(key))
  {
    kWarning() << "Key " << key << " is not registered.";
    return false;
  }
  this->creators.remove(key);
  return true;
}
