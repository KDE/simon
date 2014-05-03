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

#ifndef SIMON_DIALOGBOOLEANFIELD_H_30683738694b47f68743788cf61493e6
#define SIMON_DIALOGBOOLEANFIELD_H_30683738694b47f68743788cf61493e6

#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QSharedPointer>
#include <QCheckBox>
#include <KDebug>
#include "dialogfield.h"

class DialogBooleanField : public DialogField<bool>
{
  protected:
    virtual const QString& getType() const { return DialogBooleanField::typeInfo.id; }
    virtual QSharedPointer<VariableType> parseValue(const QString& value);

    virtual QSharedPointer<VariableType> deSerializeValue(const QDomElement& elem);
    virtual QDomElement seriaizeValue(QDomDocument * doc);

    DialogBooleanField() : DialogField< VariableType >() { }
    DialogBooleanField(const QString& n) : DialogField< VariableType >(n) { }
  public:
    static const DialogFieldTypeInfo typeInfo;
    static DialogFieldBase* deSerializeDialogBooleanField(const QDomElement& elem);
    static DialogFieldBase* createDialogBooleanField(const QString& name, const QString& value);
    static DialogFieldBase* createDialogBooleanField(const QString& name);

    DialogBooleanField(const QString& name, const VariableType& val) : DialogField<VariableType>(name,val) { }
    DialogBooleanField(const QString& name, const QSharedPointer<VariableType>& val) : DialogField<VariableType>(name,val) { }

    virtual QString toString();
};

class DialogBooleanFieldCreator : public DialogFieldCreator
{
  private:
    QSharedPointer<QCheckBox> ui;
    DialogBooleanFieldCreator() : ui(new QCheckBox()) { }
  public:
    static QSharedPointer< DialogFieldCreator > getCreatorInstance() {  return QSharedPointer<DialogFieldCreator>(new DialogBooleanFieldCreator()); }
    virtual QSharedPointer< QWidget > getUI() { return ui; }
    virtual DialogFieldBase* createField(const QString& name) const { return new DialogBooleanField(name,ui.data()->isChecked()); }
    virtual ~DialogBooleanFieldCreator() { }
};

#endif /* SIMON_DIALOGBOOLEANFIELD_H_30683738694b47f68743788cf61493e6 */
