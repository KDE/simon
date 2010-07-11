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

#include "multikcmview.h"
#include <QVBoxLayout>
#include <QIcon>
#include <KTabWidget>
#include <KGlobal>
#include <KIcon>

MultiKCMView::MultiKCMView(QWidget* parent, const QVariantList& args) : KCModule(KGlobal::mainComponent(), parent)
{
  Q_UNUSED(args);

  wgModules = new KTabWidget(this);
  QVBoxLayout *lay = new QVBoxLayout(this);
  lay->addWidget(wgModules);
  setLayout(lay);
}


void MultiKCMView::slotChanged(bool changed)
{
  KCModule *module = dynamic_cast<KCModule*>(sender());
  if (!module) return;

  moduleChangedState.replace(modules.indexOf(module), changed);

  emitChanged();
}


void MultiKCMView::emitChanged()
{
  foreach (bool changedFlag, moduleChangedState) {
    if (changedFlag) {
      emit changed(true);
      return;
    }
  }
  return changed(false);
}


void MultiKCMView::registerModule(KCModule* mod, const KIcon& icon, const QString& label)
{
  modules  << mod;

  connect(mod, SIGNAL(changed(bool)), this, SLOT(slotChanged(bool)));

  wgModules->addTab(mod, icon, label);
  moduleChangedState << false;
}


MultiKCMView::~MultiKCMView()
{
  qDeleteAll(modules);
}


void MultiKCMView::load()
{
  foreach (KCModule *m, modules)
    m->load();
}


void MultiKCMView::save()
{
  foreach (KCModule *m, modules)
    m->save();
}
