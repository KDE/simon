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

#include "firstrunbasemodelconfig.h"
#include <QPointer>
#include <KLocalizedString>
#include <KCMultiDialog>

FirstRunBaseModelConfig::FirstRunBaseModelConfig(QWidget* parent)
: QWizardPage(parent)
{
  ui.setupUi(this);
  setTitle(i18n("Base models"));
  connect(ui.pbConfigureBaseModel, SIGNAL(clicked()), this, SLOT(configureBaseModel()));
  ui.lbDescription->setText(i18n("<html><head />"
    "<body>"
    "<p>simon compares the microphone input to something called an \"acoustic model\" describing how your voice sounds like and what you are saying.</p>"
    "<p>With simon you can create and manage those models yourself or you can simply download and use general models that describe the average speaker of your target language. We call these models \"base models\".</p>"
    "<p>If you do not select a base model, you need to train one yourself by reading trainingstexts aloud (\"Training\").</p>"
    "<p>If you do select a base model you do not need any training but the recognition may be less accurate. <span style=\" font-weight:600;\">When using static base models, the installation of the HTK is not required.</span></p>"
    "<p>You can find more information on <a href=\"http://www.simon-listens.org/wiki/index.php/English:_Base_models\">base models on our wikis</a>.</p></body></html>"));

}


void FirstRunBaseModelConfig::configureBaseModel()
{
  QPointer<KCMultiDialog> configDialog = new KCMultiDialog(this);
  configDialog->addModule("simonmodelconfig", QStringList() << "");
  configDialog->exec();
  delete configDialog;
}


FirstRunBaseModelConfig::~FirstRunBaseModelConfig()
{
}
