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

#ifndef SIMON_DIALOGINTEGERFIELD_H_30683738694b47f68743788cf61493e6
#define SIMON_DIALOGINTEGERFIELD_H_30683738694b47f68743788cf61493e6

#include <QString>
#include <QDomDocument>
#include <QDomElement>
#include <QSharedPointer>
#include <KDebug>
#include <KIntSpinBox>
#include <fcntl.h>
#include "dialogfield.h"

class DialogIntegerField : public DialogField<int>
{
  protected:
    virtual const QString& getType() const { return DialogIntegerField::typeInfo.id; }
    virtual QSharedPointer<VariableType> parseValue(const QString& value);

    virtual QSharedPointer<VariableType> deSerializeValue(const QDomElement& elem);
    virtual QDomElement seriaizeValue(QDomDocument * doc);

    DialogIntegerField() : DialogField< int >() { }
    DialogIntegerField(const QString& n) : DialogField< int >(n) { }
  public:
    static const DialogFieldTypeInfo typeInfo;
    static DialogFieldBase* deSerializeDialogIntegerField(const QDomElement& elem);
    static DialogFieldBase* createDialogIntegerField(const QString& name, const QString& value);
    static DialogFieldBase* createDialogIntegerField(const QString& name);

    DialogIntegerField(const QString& name, const VariableType& val) : DialogField<int>(name,val) { }
    DialogIntegerField(const QString& name, const QSharedPointer<VariableType>& val) : DialogField<int>(name,val) { }

    virtual QString toString() { return QString::number(*getVal().data()); }
};

class DialogIntegerFieldCreator : public DialogFieldCreator
{
  private:
    QSharedPointer<KIntSpinBox> ui;
    DialogIntegerFieldCreator() : ui(new KIntSpinBox()) { }
  public:
    static QSharedPointer< DialogFieldCreator > getCreatorInstance() {  return QSharedPointer<DialogFieldCreator>(new DialogIntegerFieldCreator()); }
    virtual QSharedPointer< QWidget > getUI() { return ui; }
    virtual DialogFieldBase* createField(const QString& name) const { return new DialogIntegerField(name,ui.data()->value()); }
    virtual ~DialogIntegerFieldCreator() { }
};
#endif /* SIMON_DIALOGINTEGERFIELD_H_30683738694b47f68743788cf61493e6 */
