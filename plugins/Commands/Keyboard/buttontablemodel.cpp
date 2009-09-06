/*
 *   Copyright (C) 2009 Dominik Neumeister <neudob06@edvhtl.at>
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

#include "buttontablemodel.h"
#include "keyboardbutton.h"
#include <KDebug>

ButtonTableModel::ButtonTableModel(QObject *parent) : QAbstractTableModel(parent)
{
}

ButtonTableModel::ButtonTableModel(QList<KeyboardSet *> * setList, QComboBox* cbSets, QComboBox* cbTabs, QObject* parent) : QAbstractTableModel(parent),
    list(setList),
    cbSet(cbSets),
    cbTab(cbTabs)
{
}

void ButtonTableModel::addButton(int setId, int tabId, KeyboardButton* b)
{
	int oldSize = list->at(setId)->getTabList()->at(tabId)->getButtonList()->size();
	beginInsertRows(QModelIndex(), oldSize, 1);
	list->at(setId)->getTabList()->at(tabId)->getButtonList()->append(b);

	kDebug() << "Button has been added";
	int newSize = list->at(setId)->getTabList()->at(tabId)->getButtonList()->size();

	kDebug() << oldSize << newSize;
	endInsertRows();

	emit dataChanged(index(oldSize, 0), index(newSize, columnCount()));
}

int ButtonTableModel::rowCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    int setId = cbSet->currentIndex();
    int tabId = cbTab->currentIndex();
    if ((setId == -1) || (tabId == -1)) return 0;

    return list->at(setId)->getTabList()->at(tabId)->getButtonList()->count();
}

int ButtonTableModel::columnCount(const QModelIndex &parent) const
{
    Q_UNUSED(parent)
    return 4;
}

QVariant ButtonTableModel::data(const QModelIndex &index, int role) const
{
    if(!index.isValid())
        return QVariant();

    if(index.row() >= list->size() || index.row() < 0)
        return QVariant();

    if(role == Qt::DisplayRole) 
    {
        KeyboardButton *b = list->at(cbSet->currentIndex())->getTabList()->at(cbTab->currentIndex())->getButtonList()->at(index.row());

        if(index.column() == 0)
            return b->getTriggerShown();
        else if(index.column() == 1)
            return b->getTriggerReal();
	else if(index.column() == 2)
	    return b->getValue();
	else if(index.column() == 3)
	    return b->getValueType();
    }
    return QVariant();
}

QVariant ButtonTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if(role != Qt::DisplayRole)
        return QVariant();

    if(orientation == Qt::Horizontal) 
    {
        switch(section) 
	{
            case 0:
                return tr("Name");

            case 1:
                return tr("Trigger");

	    case 2:
		return tr("Value");

	    case 3:
		return tr("Value type");

            default:
                return QVariant();
        }
    }
    return QVariant();
}

//bool ButtonTableModel::insertRows(int position, int rows, const QModelIndex &index)
//{
//    Q_UNUSED(index);
//    beginInsertRows(QModelIndex(), position, position+rows-1);

//    for(int row=0; row<rows; row++) 
//    {
//        QPair<QString, QString> pair(" ", " ");
//        list->insert(position, pair);
//    }

//    endInsertRows();
//    return true;
//}

//bool ButtonTableModel::removeRows(int position, int rows, const QModelIndex &index)
//{
//    Q_UNUSED(index);
//    beginRemoveRows(QModelIndex(), position, position+rows-1);
//
//    for (int row=0; row < rows; ++row) 
//    {
//        list.removeAt(position);
//    }

//    endRemoveRows();
//    return true;
//}

//bool ButtonTableModel::setData(const QModelIndex &index, const QVariant &value, int role)
//{
//        if(index.isValid() && role == Qt::EditRole) 
//	{
//                int row = index.row();

//                KeyboardButton b = list->value(row);

//                if(index.column() == 0)
//                        b.first = value.toString();
//                else if(index.column() == 1)
//                        b.second = value.toString();
//        else
//            return false;

//        list->replace(row, b);
//                emit(dataChanged(index, index));

//        return true;
//        }

//        return false;
//	return true;
//}

//Qt::ItemFlags ButtonTableModel::flags(const QModelIndex &index) const
//{
//    if(!index.isValid())
//        return Qt::ItemIsEnabled;

//    return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
//}

//QList<KeyboardSet *> ButtonTableModel::getList()
//{
//    return *list;
//}

