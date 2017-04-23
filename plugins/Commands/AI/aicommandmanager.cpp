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

#include "aicommandmanager.h"
#include "aiconfiguration.h"
#include "aimlparser.h"

#include <simonlogging/logger.h>
#include <simonscenarios/scenario.h>
#include <simontts/simontts.h>

#include <QDebug>
#include <QXmlInputSource>
#include <QFile>
#include <QDir>

#include <KProcess>
#include <KProgressDialog>
#include <KLocalizedString>
#include <KPluginFactory>
#include <KStandardDirs>
#include <KDebug>

K_PLUGIN_FACTORY( AIPluginFactory,
registerPlugin< AICommandManager >();
)

K_EXPORT_PLUGIN( AIPluginFactory("simonaicommand") )

AICommandManager::AICommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
parser(0)
{
}


const QString AICommandManager::name() const
{
  return i18n("Artificial Intelligence");
}


const QString AICommandManager::iconSrc() const
{
  return ("view-media-artist");
}


CommandConfiguration* AICommandManager::getConfigurationPage() const
{
  return config;
}


bool AICommandManager::trigger(const QString& triggerName, bool silent)
{
  kDebug() << "Triggering! " << parser;
  Q_UNUSED(silent);
  Q_ASSERT(parser);
  if (!parser) return false;

  QString aliceResponse = parser->getResponse(triggerName);
  SimonTTS::say(aliceResponse);

  return true;
}


bool AICommandManager::setupParser()
{
  if (parser) {
    parser->saveVars(KStandardDirs::locate("data", "ai/util/vars.xml"));
    delete parser;
    parser=0;
  }

  QFile f(KStandardDirs::locateLocal("data", "ai/parser.log"));
  if (!f.open(QIODevice::WriteOnly)) return false;

  parser = new AIMLParser(new QTextStream(&f));
  parser->loadVars(KStandardDirs::locate("data", "ai/util/vars.xml"), false);
  parser->loadVars(KStandardDirs::locate("data", "ai/util/bot.xml"), true);
  parser->loadSubstitutions(KStandardDirs::locate("data", "ai/util/substitutions.xml"));

  QString aimlDirString = KStandardDirs::locate("data", "ai/aimls/"+static_cast<AIConfiguration*>(config)->aimlSet()+'/');

  QDir aimlDir(aimlDirString);
  QStringList aimls = aimlDir.entryList(QStringList() << "*.aiml", QDir::Files);

  KProgressDialog *dlg = new KProgressDialog(0, i18n("Artificial Intelligence"), i18n("Loading artificial intelligence..."));
  dlg->progressBar()->setMaximum(aimls.count());
  dlg->show();
  int i=0;
  foreach (const QString& aiml, aimls) {
    parser->loadAiml(aimlDirString+aiml);
    dlg->progressBar()->setValue(++i);
  }
  dlg->deleteLater();
  return true;
}


bool AICommandManager::deSerializeConfig(const QDomElement& elem)
{
  Q_UNUSED(elem);
  if (parser) return true;

  config = new AIConfiguration(parentScenario);
  static_cast<AIConfiguration*>(config)->setManager(this);
  config->deSerialize(elem);

  if (!setupParser()) return false;
  if (!SimonTTS::initialize()) return false;

  return true;
}

void AICommandManager::finalize()
{
  if (parser)
    parser->saveVars(KStandardDirs::locate("data", "ai/util/vars.xml"));
}

AICommandManager::~AICommandManager()
{
  delete parser;
}
