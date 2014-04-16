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
#include "dialogfield.h"
#include <KDebug>

//Ben Notes:  An instance of this class is essentially going to be a list
//            of active variables for the current Dialog Instance.
//TODO: Add documentation

class DialogFieldStore {
  private:
    QHash<QString,DialogFieldBase*> dialogFields;
    QHash<QString,DialogFieldTypeInfo const *> creators;
  public:
    //TODO: Figure out a better solution to this
    DialogFieldStore() { registerDefaults(); }
    bool removeField(const QString& name);
    int count() const { return dialogFields.count(); }
    bool contains(QString s) { return dialogFields.contains(s); }
    bool registerFactory(const DialogFieldTypeInfo& dfti);
    bool unregisterFactory(const QString& key);

    bool registerDefaults();

    //bool addField(const QString& name);
    bool addField(const QString& type, const QString& name, const QString& value);


    template <typename T>
    DialogFieldValue<T> getValue(const QString& name) const
    {
	return this->dialogFields[name]->getValue<T>();
    }
};
