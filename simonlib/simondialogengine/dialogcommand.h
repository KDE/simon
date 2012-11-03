/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_DIALOGCOMMAND_H_1822E03B797C494599B9629C6B47E478
#define SIMON_DIALOGCOMMAND_H_1822E03B797C494599B9629C6B47E478

#include <simonscenarios/command.h>
#include <QDomElement>
#include <QString>
#include <QObject>
#include <QStringList>
#include "simondialogengine_export.h"
#include <KUrl>
class QDomDocument;
class QTimer;

/**
 *	@class DialogCommand
 *	@brief Ressembles one exec-command
 *
 *	@version 0.1
 *	@date 19.05.2008
 *	@author Peter Grasch
 */
class SIMONDIALOGENGINE_EXPORT DialogCommand : public QObject, public Command
{
  Q_OBJECT

  signals:
    void requestDialogState(int newState);
    void changed();

  private:
    QString m_text;
    bool m_showIcon;

    bool m_silent;

    bool m_activateAutomatically;
    int m_activateAfter;
    bool m_changeDialogState;
    int m_nextDialogState;
    bool m_executeCommands;
    QStringList m_commands;
    QStringList m_commandTypes;

    QTimer *m_autoTimer;

  private slots:
    void autoTrigger();
 
  protected:
    const QMap<QString,QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    DialogCommand();

  public:
    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const KIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
    bool deSerializePrivate(const QDomElement& commandElem);

    DialogCommand(const QString& name, const QString& iconSrc, const QString& description,
        const QString& text, bool showIcon, bool silent, bool triggerAutomatically, 
        int triggerAfter, bool changeDialogState, int nextDialogState, bool executeCommands, 
        const QStringList& commands, const QStringList& commandTypes);

    void update(const QString& name, const QString& iconSrc, const QString& description,
        const QString& text, bool showIcon, bool silent, bool triggerAutomatically, 
        int triggerAfter, bool changeDialogState, int nextDialogState, bool executeCommands, 
        const QStringList& commands, const QStringList& commandTypes);

    void left();
    void presented();

    QString text() { return m_text; }
    bool showIcon() { return m_showIcon; }

    bool silent() { return m_silent; }
    bool changeDialogState() { return m_changeDialogState; }
    int nextDialogState() { return m_nextDialogState; }

    bool executeCommands() { return m_executeCommands; }

    bool activateAutomatically() { return m_activateAutomatically; }
    int activationTimeout() { return m_activateAfter; }

    QStringList getCommands() { return m_commands; }
    QStringList getCommandTypes() { return m_commandTypes; }

    void createStateLink(int thisState);

    ~DialogCommand();

    STATIC_CREATE_INSTANCE_H(DialogCommand);
};
#endif
