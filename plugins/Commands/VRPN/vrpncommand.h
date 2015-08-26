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

#ifndef SIMON_VRPNCOMMAND_H_1822E03B797C494599B9629C6B47E478
#define SIMON_VRPNCOMMAND_H_1822E03B797C494599B9629C6B47E478

#include <simonscenarios/command.h>
#include <QDomElement>
class QDomDocument;

class VRPNCommand : public Command
{

  public:
    enum ClickMode {
      PressAndRelease=0,
      Press=1,
      Release=2,
      Toggle=3
    };
    static const QString staticCategoryText();
    static const QIcon staticCategoryIcon();

    const QIcon getCategoryIcon() const;
    const QString getCategoryText() const;

    QDomElement serializePrivate(QDomDocument *doc, QDomElement& commandElem);
    bool deSerializePrivate(const QDomElement& commandElem);

    VRPNCommand(const QString& name, ClickMode clickMode, const QString& iconSrc, const QString& description, const QString& button) :
      Command(name, iconSrc, description), button(button), clickMode(clickMode)
    { }

    ~VRPNCommand() {}

    QString getButton() const
    {
      return button;
    }

    ClickMode getClickMode() const
    {
      return clickMode;
    }

    STATIC_CREATE_INSTANCE_H(VRPNCommand);
  protected:
    QString button;
    ClickMode clickMode;
    const QMap<QString,QVariant> getValueMapPrivate() const;
    bool triggerPrivate(int *state);
    VRPNCommand() {}
};
#endif
