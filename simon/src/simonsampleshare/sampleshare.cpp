/*
 *   Copyright (C) 2011 Alessandro Buggin <alessandrobuggin@gmail.com>
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
/*
 * create a new library next to simonappcore, simonactionsui, etc. called "simonsampleshare";
 * start by creating a ui (KDialog), that simply says "Hello World";
 * integrate this library and dialog into simon by adding 
 * a menu option to "Actions" called "Publish samples" 
 * displaying the dialog from the new library
 */
// #include <KDialogButtonBox>
#include "sampleshare.h"
SampleShare::SampleShare(QWidget* parent) :
KDialog(parent){
  QWidget *widget = new QWidget( this );
  ui.setupUi(widget);
  ui.label->setText("Hello World!");
  initDisplay();
}