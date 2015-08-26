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

#ifndef SIMON_JSONCOMMAND_H_1822E03B797C494599B9629C6B47E478
#define SIMON_JSONCOMMAND_H_1822E03B797C494599B9629C6B47E478

#include <simonscenarios/command.h>
#include <QDomElement>
class QDomDocument;

/**
 *	@class JsonCommand
 *	@brief Ressembles one exec-command
 *
 *	@version 0.1
 *	@date 19.05.2008
 *	@author Peter Grasch
 */
class JsonCommand : public Command
{
  private:
    QString m_url;
    QString m_request;

  protected:
    const QMap<QString,QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    JsonCommand() {}

  public:
    static const QString staticCategoryText();
    static const QIcon staticCategoryIcon();

    const QIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
    bool deSerializePrivate(const QDomElement& commandElem);

    JsonCommand(const QString& name, const QString& iconSrc, const QString& description, const QString& url, const QString& request) :
    Command(name, iconSrc, description), m_url(url), m_request(request) {
    }

    ~JsonCommand() {}

    QString url();
    QString request();

    STATIC_CREATE_INSTANCE_H(JsonCommand);
};

#endif

