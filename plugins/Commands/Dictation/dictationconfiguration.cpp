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

#include "dictationconfiguration.h"
#include "replacements.h"
#include "dictationreplacementui.h"
#include <QVariantList>
#include <QModelIndex>
#include <KDebug>
#include <kgenericfactory.h>
#include <KAboutData>
#include <KMessageBox>

K_PLUGIN_FACTORY_DECLARATION(DictationCommandPluginFactory)

DictationConfiguration::DictationConfiguration(Scenario *parent, const QVariantList &args)
  : CommandConfiguration(parent, "dictation", ki18n( "Dictation" ),
  "0.1", ki18n("Dictation"),
  "text-field",
  DictationCommandPluginFactory::componentData()),
  m_replacements(new Replacements)
{
  Q_UNUSED(args);
  ui.setupUi(this);

  ui.tvReplacements->setModel(m_replacements);

  ui.pbAddReplacement->setIcon(KIcon("list-add"));
  ui.pbRemoveReplacement->setIcon(KIcon("list-remove"));
  ui.pbEditReplacement->setIcon(KIcon("list-edit"));

  QObject::connect(ui.leAppendText, SIGNAL(textChanged(QString)), this, SLOT(slotChanged()));
  QObject::connect(ui.pbRemoveReplacement, SIGNAL(clicked()), this, SLOT(removeReplacement()));
  QObject::connect(ui.pbEditReplacement, SIGNAL(clicked()), this, SLOT(editReplacement()));
  QObject::connect(ui.pbAddReplacement, SIGNAL(clicked()), this, SLOT(addReplacement()));
}


bool DictationConfiguration::deSerialize(const QDomElement& elem)
{
  QString text = elem.firstChildElement("postText").attribute("value");
  ui.leAppendText->setText(text);
  m_replacements->deSerialize(elem.firstChildElement("replacements"));
  return true;
}


QDomElement DictationConfiguration::serialize(QDomDocument *doc)
{
  QDomElement configElem = doc->createElement("config");
  QDomElement postTextElem = doc->createElement("postText");
  postTextElem.setAttribute("value", ui.leAppendText->text());
  configElem.appendChild(postTextElem);
  configElem.appendChild(m_replacements->serialize(doc));
  return configElem;
}


QString DictationConfiguration::appendText() const
{
  return ui.leAppendText->text();
}

Replacements* DictationConfiguration::replacements() const
{
  return m_replacements;
}


void DictationConfiguration::defaults()
{
  ui.leAppendText->setText(" ");
  m_replacements->defaults();
}

void DictationConfiguration::addReplacement()
{
  QPointer<DictationReplacementUi> ui(new DictationReplacementUi(this));
  Replacement *r = ui->add();
  if (r)
    m_replacements->add(r);
  emit changed(true);
}

void DictationConfiguration::editReplacement()
{
  QPointer<DictationReplacementUi> ui(new DictationReplacementUi(this));
  Replacement *old = getCurrentlySelectedRecommendation();
  if (!old) return;
  ui->init(old);
  Replacement *r = ui->add();
  if (r) {
    m_replacements->remove(old);
    m_replacements->add(r);
  }
  emit changed(true);
}

void DictationConfiguration::removeReplacement()
{
  Replacement *r = getCurrentlySelectedRecommendation();
  if (KMessageBox::questionYesNoCancel(this, i18n("Do you really want to remove the selected replacement?")) == KMessageBox::Yes) {
    m_replacements->remove(r);
    emit changed(true);
  }
}

Replacement* DictationConfiguration::getCurrentlySelectedRecommendation() const
{
  QModelIndex i = ui.tvReplacements->currentIndex();
  if (!i.isValid()) {
    KMessageBox::information(0, i18n("Please select a replacement from the list."));
    return 0;
  }
  return static_cast<Replacement*>(i.internalPointer());
}


DictationConfiguration::~DictationConfiguration()
{

}
