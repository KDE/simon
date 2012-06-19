/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#include "samplegroupitemdelegate.h"
#include "simoncontextdetection/contextmanager.h"
#include "simonscenarios/trainingmanager.h"
#include "simonsound/soundserver.h"
#include <KComboBox>
#include <KDebug>

SampleGroupItemDelegate::SampleGroupItemDelegate(SampleGroupCondition *sampleGroupCondition, QObject *parent) :
    QStyledItemDelegate(parent),
    m_sampleGroupCondition(sampleGroupCondition)
{
}

QWidget* SampleGroupItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    Q_UNUSED(option)
    Q_UNUSED(index)

    KComboBox *sampleGroupBox = new KComboBox(parent);

    sampleGroupBox->setEditable(true);

    QStringList availableSampleGroups;

    availableSampleGroups << SoundServer::defaultSampleGroup();
    availableSampleGroups << SoundServer::getInputDeviceDefaultSampleGroups();
    availableSampleGroups << TrainingManager::getInstance()->getPrompts()->sampleGroups();
    availableSampleGroups << m_sampleGroupCondition->getSampleGroups();

    availableSampleGroups.removeDuplicates();
    sampleGroupBox->addItems(availableSampleGroups);
    sampleGroupBox->setCurrentIndex(0);

    return sampleGroupBox;
}

void SampleGroupItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    KComboBox *sampleGroupBox = qobject_cast<KComboBox*>(editor);

    QString sampleGroup = index.data().toString();

    if (sampleGroupBox->findText(sampleGroup) >= 0)
    {
        sampleGroupBox->setCurrentIndex(sampleGroupBox->findText(sampleGroup));
    }
    else
    {
        sampleGroupBox->addItem(sampleGroup);
        sampleGroupBox->setCurrentIndex(sampleGroupBox->findText(sampleGroup));
    }
}

void SampleGroupItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    KComboBox *sampleGroupBox = qobject_cast<KComboBox*>(editor);
    QString sampleGroup = sampleGroupBox->currentText();

    kDebug() << "setting current model data" << sampleGroup;
    kDebug() << "setting current model data" << model;

    model->setData(index, QVariant(sampleGroup));

    if (sampleGroupBox->findText(sampleGroup) < 0)
    {
        sampleGroupBox->addItem(sampleGroup);
        sampleGroupBox->setCurrentIndex(sampleGroupBox->findText(sampleGroup));
    }
}
