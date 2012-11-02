/*
 *   Copyright (C) 2009 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_DIALOGCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_DIALOGCOMMANDMANAGER_H_7A7B9100FF5245329569C1B540119C37

#include <simonscenarios/commandmanager.h>
#include <simonactions/greedyreceiver.h>
#include <simondialogengine/dialogmanager.h>
#include <QVariantList>
#include <QList>
#include <KXMLGUIClient>

class Avatar;
class QWidget;
class KAction;
class DialogConfiguration;
class DialogView;
class DialogState;
class DialogTextParser;

/**
 *	@class DialogCommandManager
 *	@brief Manager for the dialog commands
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class DialogCommandManager : public CommandManager, public GreedyReceiver, public DialogManager
{
  Q_OBJECT

  private:
    KAction *activateAction;

    DialogState *currentDialogSate;

    DialogTextParser *dialogParser;
    QList<DialogView*> dialogViews;
    QList<DialogState*> dialogStates;

    void initState(DialogState *state);


  private slots:
    void bindStateCommands();
    void deregister();

    void stateChanged();
    void stateDestroyed();
    
    DialogConfiguration* getDialogConfiguration() const;

  public slots:
    void initState(int state);
    bool greedyTrigger(const QString& inputText);
    void activate(const QString& arg0="", const QString& arg1="", const QString& arg2="", const QString& arg3="",
      const QString& arg4="", const QString& arg5="", const QString& arg6="",
      const QString& arg7="", const QString& arg8="", const QString& arg9="");

  protected:
    bool shouldAcceptCommand(Command *command);

  public:
    const QString preferredTrigger() const { return QString(); }
    const QString iconSrc() const;
    const QString name() const;

    QDomElement serializeCommands(QDomDocument *doc);
    bool deSerializeCommandsPrivate(const QDomElement& elem);

    bool deSerializeConfig(const QDomElement& elem);
    void setFont(const QFont& font);

    QList<DialogState*> getStates() const { return dialogStates; }

    bool addState(const QString& name);
    bool removeState(DialogState *state);

    bool moveStateUp(DialogState *state);
    bool moveStateDown(DialogState *state);

    virtual bool trigger(const QString& triggerName, bool silent);

    QString getOptionSeparatorText() const;
    QString getRepeatAnnouncement() const;
    bool getRepeatOnInvalidInput() const;
    
    Avatar* getAvatar(int id) const;
    int getAvatarSize() const;
    bool getDisplayAvatarNames() const;

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    DialogCommandManager(QObject* parent, const QVariantList& args);

    ~DialogCommandManager();

};
#endif
