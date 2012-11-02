/*
 *   Copyright (C) 2011 Peter Grasch <peter.grasch@bedahr.org>
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


#ifndef LANGUAGEPROFILEVIEW_H
#define LANGUAGEPROFILEVIEW_H

#include <KDialog>

class KProcess;
class GraphemeToPhoneme;
namespace Ui {
  class LanguageProfileView;
}

class LanguageProfileView : public KDialog
{
  Q_OBJECT
  
private:
  Ui::LanguageProfileView *ui;
  GraphemeToPhoneme *g2p;
  
private slots:
  void createProfile();
  
  void displayState(const QString& state, int now, int max);
  void success(const QString& path);
  void failed();
  
  virtual void slotButtonClicked(int button);
  
public:
    explicit LanguageProfileView(QWidget* parent = 0, Qt::WFlags flags = 0);
    virtual void done(int);
    virtual ~LanguageProfileView();
};

#endif // LANGUAGEPROFILEVIEW_H
