/*
 *   Copyright (C) 2010 Peter Grasch <grasch@simon-listens.org>
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

#ifndef SIMON_DIALOGVIEW_H_7A7B9100FF5245329569C1B540119C37
#define SIMON_DIALOGVIEW_H_7A7B9100FF5245329569C1B540119C37

class QFont;
class DialogCommandManager;
class DialogState;
class QString;

class DialogView
{
  protected:
    DialogCommandManager *m_dialog;

  public:
    DialogView(DialogCommandManager *dialog) : m_dialog(dialog) {}

    virtual bool start()=0;
    virtual bool stop()=0;

    virtual bool present(const DialogState& state)=0;

    virtual void setFont(const QFont& /*font*/) {}

    virtual void correctInputReceived() {};
    virtual void warnOfInvalidInput(const QString& /*input*/) {}
    virtual void repeat(const DialogState& /*state*/) {}

    virtual ~DialogView() {}
};

#endif

