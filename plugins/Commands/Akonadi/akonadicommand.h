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

#ifndef SIMON_AKONADICOMMAND_H_1822E03B797C494599B9629C6B47E478
#define SIMON_AKONADICOMMAND_H_1822E03B797C494599B9629C6B47E478

#include <simonscenarios/command.h>
#include <QDomElement>
#include <QObject>
#include <KUrl>
#include <KDateTime>

class KJob;class QDomDocument;

class AkonadiCommand : public QObject, public Command
{
  Q_OBJECT
  
  public:
    enum TimerType
    {
      Absolute=1,
      Relative=2
    };
    enum RelativeDurationDimension
    {
      Seconds=1,
      Minutes=2,
      Hours=3,
      Days=4
    };
    
  private:
    QString m_command;
    QString m_commandType;
    
    TimerType m_type;
    KDateTime m_executeAt;
    int m_relativeDuration;
    
    KDateTime calculateExecutionTime();
    
  private slots:
    void storeJobFinished(KJob*);
    
  protected:
    const QMap<QString,QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    AkonadiCommand() : QObject() {}

  public:
    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const KIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
    bool deSerializePrivate(const QDomElement& commandElem);
    
    void getRelativeTime(AkonadiCommand::RelativeDurationDimension& dimension, int& value) const;

    AkonadiCommand(const QString& name, const QString& iconSrc, const QString& description,
      QString command, QString commandType, TimerType type, KDateTime executeAt, int relativeDuration
    ) :
    QObject(), Command(name, iconSrc, description),
    m_command(command), m_commandType(commandType),
    m_type(type), m_executeAt(executeAt), m_relativeDuration(relativeDuration)
    {
    }
    
    QString getChildTrigger() { return m_command; }
    QString getChildType() { return m_commandType; }
    TimerType getType() { return m_type; }
    KDateTime getAbsoluteExecutionTime() { return m_executeAt; }

    ~AkonadiCommand() {}

    STATIC_CREATE_INSTANCE_H(AkonadiCommand);
};
#endif
