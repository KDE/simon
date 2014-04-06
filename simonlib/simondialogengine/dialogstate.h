/*
 *   Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
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

#include "simondialogengine_export.h"
#include <simonscenarios/command.h>
#include <simondialogengine/dialogtextparser.h>
#include <QList>
#include <QAbstractItemModel>
#include <QString>

class DialogCommand;
class DialogText;
class DialogTurn;
class DialogTextParser;
class QDomElement;
class QDomDocument;

class SIMONDIALOGENGINE_EXPORT DialogState : public QAbstractItemModel
{
  Q_OBJECT
  signals:
    void requestDialogState(int state);
    void changed();

  private slots:
    void turnChanged();
    void turnDestroyed();

  private:
    QString m_name;
    int m_currentRandomTurnIndex;
    DialogTextParser *m_parser;    
    QList<DialogTurn*> m_turns;
    DialogTurn* currentDialogTurn;
    
    bool deSerialize(DialogTextParser *parser, const QDomElement& elem);
    DialogState(QObject *parent=0) : QAbstractItemModel(parent) {}

  public:
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant headerData(int, Qt::Orientation orientation,
                      int role = Qt::DisplayRole) const;
    QObject* parent() { return QObject::parent(); }
    QModelIndex parent(const QModelIndex &index) const;
    int rowCount(const QModelIndex &parent = QModelIndex()) const;

    QModelIndex index(int row, int column,const QModelIndex &parent = QModelIndex()) const;
    virtual QVariant data(const QModelIndex &index, int role) const;
    virtual int columnCount(const QModelIndex &parent = QModelIndex()) const;

    DialogState(DialogTextParser *parser, const QString& name, QList<DialogTurn*> turns, QObject *parent=0);

    QString getName() const { return m_name; }
    int getTextCount();
    void setCurrentTurn(int index);
    
    int addText(const QString& text);
    bool removeText(int id);
    
    QString getText() const;
    QString getRawText(int index) const;

    QList<DialogCommand*> getTransitions() const;

    QList<DialogTurn*> getTurns() const { return m_turns; }
    DialogTurn* getCurrentTurn() { return currentDialogTurn; }
    DialogTurn* createTurn();
    void addTurn(DialogTurn* turn);
    bool removeTurn();
    void setTurn(DialogTurn* turn, int index);
    void bindStateCommands(QList<Command*> commands);

    static DialogState* createInstance(DialogTextParser *parser, const QDomElement& elem);
    QDomElement serialize(QDomDocument *doc);

    void addTransition(DialogCommand* command);
    void removeTransition(DialogCommand* command);

    bool moveTransitionUp(DialogCommand* command);
    bool moveTransitionDown(DialogCommand* command);

    bool rename(const QString& newName);
    bool setRawText(int index, const QString& data);

    void setSilence(bool silence);
    void setAnnounceRepeat(bool announce);
    bool silence() const;
    bool announceRepeat() const;
    
    bool getDisplayAvatar() const;
    void setDisplayAvatar(bool display);
    
    int getAvatarId() const;
    void setAvatar(int id);
    
    void updateRandomTextSelection();

    void presented();
    void left();

    ~DialogState();
};

#endif

