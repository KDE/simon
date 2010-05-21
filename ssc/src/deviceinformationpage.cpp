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
#include "deviceinformationpage.h"
#include "deviceinformationwidget.h"
#include "sscconfig.h"

#include <simonsound/soundserver.h>

#include <QVBoxLayout>
#include <QLabel>
#include <QScrollArea>
#include <KLocalizedString>



DeviceInformationPage::DeviceInformationPage(QWidget *parent) : QWizardPage(parent),
	scrollWidget(new QScrollArea(this))
{
	setTitle(i18n("Device information"));

	QVBoxLayout *lay = new QVBoxLayout(this);

	QLabel *lbIntro = new QLabel(this);
	lbIntro->setWordWrap(true);
	lbIntro->setText(i18n("Please provide some info about your recording devices."));


	QWidget *scrollChildWidget = new QWidget(scrollWidget);
	QVBoxLayout *childLay = new QVBoxLayout(scrollChildWidget);
	scrollChildWidget->setLayout(childLay);

	scrollWidget->setWidget(scrollChildWidget);
	scrollWidget->setWidgetResizable(true);

	lay->addWidget(lbIntro);
	lay->addWidget(scrollWidget);

	setLayout(lay);
}

void DeviceInformationPage::initializePage()
{
	QList<SimonSound::DeviceConfiguration> devices = SoundServer::getTrainingInputDevices();
	foreach (const SimonSound::DeviceConfiguration& device, devices)
	{
		DeviceInformationWidget *wg = new DeviceInformationWidget(this);
		wg->setup(device);

		connect(wg, SIGNAL(completeChanged()), this, SIGNAL(completeChanged()));

		informationWidgets << wg;

		QVBoxLayout *layout = dynamic_cast<QVBoxLayout*>(scrollWidget->widget()->layout());
		if (!layout)
			continue;
		layout->addWidget(wg);
		wg->show();
	}
}

bool DeviceInformationPage::isComplete() const
{
	bool complete = true;
	foreach (DeviceInformationWidget *wg, informationWidgets)
		complete &= wg->isComplete();
	return complete;
}

#include <KDebug>

bool DeviceInformationPage::validatePage()
{
	kDebug() << "Calling storeconfig";
	foreach (DeviceInformationWidget *wg, informationWidgets)
		wg->storeConfig();

	SSCConfig::self()->writeConfig();
	return true;
}

DeviceInformationPage::~DeviceInformationPage()
{
}


