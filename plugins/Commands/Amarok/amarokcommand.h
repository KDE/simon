/*
 *   Copyright (C) 2014 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_AMAROKCOMMAND_H_1822E03B797C494599B9629C6B47E478
#define SIMON_AMAROKCOMMAND_H_1822E03B797C494599B9629C6B47E478

#include <simonscenarios/command.h>
#include <QDomElement>
#include <KUrl>
class QDomDocument;


class AmarokCommand : public Command
{
  public:
    static const QString staticCategoryText();
    static const KIcon staticCategoryIcon();

    const KIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
    bool deSerializePrivate(const QDomElement& commandElem);

    AmarokCommand(const QString& name, const QString& iconSrc, const QString& description, const QString& path) :
    Command(name, iconSrc, description), m_path(path) {
    }

    ~AmarokCommand() {}

    QString getPath() const { return m_path; }

    STATIC_CREATE_INSTANCE_H(AmarokCommand);

  protected:
    const QMap<QString,QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    AmarokCommand() {}

  private:
    QString m_path;
};
#endif
