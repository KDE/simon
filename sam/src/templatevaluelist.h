/*
 *  Copyright (C) 2010 Peter Grasch <peter.grasch@bedahr.org>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License version 2,
 *  or (at your option) any later version, as published by the Free
 *  Software Foundation
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details
 *
 *  You should have received a copy of the GNU General Public
 *  License along with this program; if not, write to the
 *  Free Software Foundation, Inc.,
 *  51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 */

#ifndef SIMON_TEMPLATEVALUELIST_H_4002119636CC42C68FE07273F9000A73
#define SIMON_TEMPLATEVALUELIST_H_4002119636CC42C68FE07273F9000A73

#include <QString>
#include <QHash>
#include <QList>

class TemplateValueList
{
  private:
    QString m_id;
    QHash<QString,QString> m_values;
    QList<TemplateValueList*> m_children;

  public:
    TemplateValueList(const QString& id);

    QString id() { return m_id; }

    void add(const QString& key,const QString& value);
    QHash<QString,QString> values();
    
    bool hasChildren() const;
    void addChild(TemplateValueList* values);
    QList<TemplateValueList*> children() const;

    ~TemplateValueList();

};

#endif

