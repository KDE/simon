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

#ifndef SIMON_OUTPUTCONFIGURATION_H
#define SIMON_OUTPUTCONFIGURATION_H

#include "simondialogengineconfui_export.h"
#include <QWidget>
#include <QDomElement>

namespace Ui {
  class OutputConfigurationDlg;
}


class SIMONDIALOGENGINECONFUI_EXPORT OutputConfiguration : public QWidget
{
private:
  Ui::OutputConfigurationDlg *ui;

public:
    explicit OutputConfiguration(QWidget* parent = 0);
    int getAvatarSize() const;
    bool getDisplayAvatarNames() const;

    bool useGUIOutput() const;
    bool useTTSOutput() const;
    QString getOptionSeparatorText() const;
    QString getRepeatAnnouncement() const;
    QStringList getRepeatTriggers() const;
    bool getRepeatOnInvalidInput() const;
    
    
    virtual bool deSerialize(const QDomElement&);
    virtual QDomElement serialize(QDomDocument *doc);
    virtual void defaults();
};

#endif // OUTPUTCONFIGURATION_H
