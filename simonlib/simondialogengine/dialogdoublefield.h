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

#ifndef SIMON_DIALOGDOUBLEFIELD_H_30683738694b47f68743788cf61493e6
#define SIMON_DIALOGDOUBLEFIELD_H_30683738694b47f68743788cf61493e6

#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QSharedPointer>
#include <KDebug>
#include <KIntSpinBox>
#include "dialogfield.h"

class DialogDoubleField : public DialogField<double>
{
  protected:
    virtual const QString& getType() const { return DialogDoubleField::typeInfo.id; }
    virtual QSharedPointer<VariableType> parseValue(const QString& value);

    virtual QSharedPointer<VariableType> deSerializeValue(const QDomElement& elem);
    virtual QDomElement seriaizeValue(QDomDocument * doc);

    DialogDoubleField() : DialogField< VariableType >() { }
    DialogDoubleField(const QString& n) : DialogField< VariableType >(n) { }
  public:
    static const DialogFieldTypeInfo typeInfo;
    static DialogFieldBase* deSerializeDialogDoubleField(const QDomElement& elem);
    static DialogFieldBase* createDialogDoubleField(const QString& name, const QString& value);
    static DialogFieldBase* createDialogDoubleField(const QString& name);

    DialogDoubleField(const QString& name, const VariableType& val) : DialogField<VariableType>(name,val) { }
    DialogDoubleField(const QString& name, const QSharedPointer<VariableType>& val) : DialogField<VariableType>(name,val) { }

    virtual QString toString() { return QString::number(*getVal().data()); }
};

class DialogDoubleFieldCreator : public DialogFieldCreator
{
  private:
    QSharedPointer<KDoubleNumInput> ui;
    DialogDoubleFieldCreator() : ui(new KDoubleNumInput()) { }
  public:
    static QSharedPointer< DialogFieldCreator > getCreatorInstance() {  return QSharedPointer<DialogFieldCreator>(new DialogDoubleFieldCreator()); }
    virtual QSharedPointer< QWidget > getUI() { return ui; }
    virtual DialogFieldBase* createField(const QString& name) const { return new DialogDoubleField(name,ui.data()->value()); }
    virtual ~DialogDoubleFieldCreator() { }
};
#endif /* SIMON_DIALOGDOUBLEFIELD_H_30683738694b47f68743788cf61493e6 */
