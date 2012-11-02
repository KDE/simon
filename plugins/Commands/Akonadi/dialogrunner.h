/*   Copyright (C) 2010 Grasch Peter <peter.grasch@bedahr.org>
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

#ifndef DIALOGRUNNER_H
#define DIALOGRUNNER_H

#include <QObject>
#include <QList>
#include <simonactions/greedyreceiver.h>
#include <simondialogengine/dialogmanager.h>

class AkonadiConfiguration;
class DialogState;
class DialogTextParser;
class DialogView;

class DialogRunner : public QObject, public GreedyReceiver, public DialogManager
{
  Q_OBJECT
private:
  AkonadiConfiguration* m_config;

  DialogTextParser *m_dialogParser;
  DialogState* m_state;
  QList<DialogView*> m_dialogViews;

public:
    virtual bool greedyTrigger(const QString& sentence);
    virtual bool getRepeatOnInvalidInput() const;
    virtual QString getRepeatAnnouncement() const;
    virtual QString getOptionSeparatorText() const;
    virtual bool getDisplayAvatarNames() const;
    virtual int getAvatarSize() const;
    virtual Avatar* getAvatar(int id) const;
protected:
    virtual void initState(DialogState* state);
    
private slots:  
    virtual void initState(int state);
    
public slots:
    void run();

public:
    DialogRunner(AkonadiConfiguration* config, DialogTextParser *parser, DialogState* states);
    virtual ~DialogRunner();
};

#endif // DIALOGRUNNER_H
