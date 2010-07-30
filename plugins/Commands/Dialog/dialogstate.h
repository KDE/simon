/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_DIALOGSTATE_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_DIALOGSTATE_H_7A7B9100FF5245329569C1B540119C37

#include <QList>
#include <QAbstractItemModel>
#include <QString>

class DialogCommand;
class DialogText;
class DialogTextParser;
class QDomElement;
class QDomDocument;

class DialogState : public QAbstractItemModel
{
  Q_OBJECT
  signals:
    void requestDialogState(int state);
    void changed();

  private:
    QString m_name;
    DialogText *m_text;
    QList<DialogCommand*> m_transitions;
    bool deSerialize(DialogTextParser *parser, const QDomElement& elem);
    DialogState(QObject *parent=0) : QAbstractItemModel(parent) {}

  protected:
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
    QObject* parent() { return QObject::parent(); }
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;



  public:
    DialogState(DialogTextParser *parser, const QString& name, const QString& text, 
        QList<DialogCommand*> transitions, QObject *parent=0);

    QString getName() const { return m_name; }
    QString getText() const;
    QString getRawText() const;
    QList<DialogCommand*> getTransitions() const { return m_transitions; }

    static DialogState* createInstance(DialogTextParser *parser, const QDomElement& elem);
    QDomElement serialize(QDomDocument *doc);

    void addTransition(DialogCommand* command);
    void removeTransition(DialogCommand* command);

    bool rename(const QString& newName);
    bool setRawText(const QString& data);

    ~DialogState();
};

#endif

