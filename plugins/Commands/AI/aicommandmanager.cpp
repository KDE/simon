/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
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
#include <simonlogging/logger.h>
#include <KLocalizedString>
#include <KGenericFactory>
#include <KStandardDirs>
#include <QDebug>
#include <KProcess>
#include <KProgressDialog>
#include <simonscenarios/scenario.h>
#include "aiconfiguration.h"
#include "aimlparser.h"

K_PLUGIN_FACTORY( AIPluginFactory,
registerPlugin< AICommandManager >();
)

K_EXPORT_PLUGIN( AIPluginFactory("simonaicommand") )

AICommandManager::AICommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
parser(0),
festivalProc(0)
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


CommandConfiguration* AICommandManager::getConfigurationPage()
{
  return config;
}


bool AICommandManager::trigger(const QString& triggerName)
{
  Q_ASSERT(parser);
  Q_ASSERT(festivalProc);
  if (!parser) return false;
  if (!festivalProc) return false;

  QString aliceResponse = parser->getResponse(triggerName);
  festivalProc->write("(SayText \""+aliceResponse.toLatin1().trimmed().replace("\"", "")+"\")\n");

  // 	QString outputName = KStandardDirs::locateLocal("tmp", "tmpSoundOutput.wav");
  // 	festivalProc->setShellCommand("echo \""+aliceResponse.toLatin1().trimmed()+"\" | "+
  // 					KStandardDirs::findExe("txt2pho").toLatin1()+" | "+
  // 					KStandardDirs::findExe("mbrola").toLatin1()+
  // 					" /opt/mbrola/de5/de5 - "+
  // 					outputName.toLatin1()+" && "+
  // 					"aplay -q -c 1 "+outputName.toLatin1());
  // 	festivalProc->execute();

  return false;                                   // pass this recognition result on to the other plugins
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

  QString aimlDirString = KStandardDirs::locate("data", "ai/aimls/"+static_cast<AIConfiguration*>(config)->aimlSet()+"/");

  QDir aimlDir(aimlDirString);
  QStringList aimls = aimlDir.entryList(QStringList() << "*.aiml", QDir::Files);

  KProgressDialog *dlg = new KProgressDialog(0, i18n("Artificial Intelligence"), i18n("Loading artificial intelligence..."));
  dlg->progressBar()->setMaximum(aimls.count());
  dlg->show();
  int i=0;
  foreach (QString aiml, aimls) {
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

  festivalProc = new KProcess(this);
  festivalProc->setProgram(KStandardDirs::findExe("festival"));
  festivalProc->start();
  if (!festivalProc->waitForStarted(1000))
    return false;

  festivalProc->write("(voice_us2_mbrola)\n");
  return true;
}


AICommandManager::~AICommandManager()
{
  if (festivalProc && (festivalProc->state()==QProcess::Running))
    festivalProc->write("(quit)\n");

  if (parser) {
    parser->saveVars(KStandardDirs::locate("data", "ai/util/vars.xml"));
    delete parser;
    parser=0;
  }
}
