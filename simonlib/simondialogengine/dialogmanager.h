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

#ifndef DIALOGMANAGER_H_7A7B9100FF5245329562C1B540119C37
#define DIALOGMANAGER_H_7A7B9100FF5245329562C1B540119C37
#include "simondialogengine_export.h"

class Avatar;
class DialogState;

class SIMONDIALOGENGINE_EXPORT DialogManager
{
protected:
  virtual void initState(DialogState *state)=0;
public:
  virtual void initState(int state)=0;
  virtual Avatar* getAvatar(int id) const=0;
  virtual int getAvatarSize() const=0;
  virtual bool getDisplayAvatarNames() const=0;
  
  virtual QString getOptionSeparatorText() const=0;
  virtual QString getRepeatAnnouncement() const=0;
  virtual bool getRepeatOnInvalidInput() const=0;

  virtual ~DialogManager() {}
};

#endif // DIALOGMANAGER_H
