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

#include "deviceinformationwidget.h"
#include "ui_deviceinformation.h"
#include "sscdaccess.h"
#include "sscconfig.h"

DeviceInformationWidget::DeviceInformationWidget(QWidget* parent) : 
	QWidget(parent),
	ui(new Ui::DeviceInformationWidget())
{
	ui->setupUi(this);
	connect(ui->cbModel, SIGNAL(editTextChanged(QString)), this, SIGNAL(completeChanged()));
	connect(ui->cbType, SIGNAL(editTextChanged(QString)), this, SIGNAL(completeChanged()));
}

void DeviceInformationWidget::setup(const SimonSound::DeviceConfiguration& device)
{
	//TODO: get models and types from server
	
	QStringList keys = SSCConfig::deviceNameKeys();
	QStringList values = SSCConfig::deviceNameValues();
	QStringList types = SSCConfig::deviceNameTypes();

	Q_ASSERT(keys.count() == values.count());
	Q_ASSERT(keys.count() == types.count());

	QString deviceName = device.name();
	QString deviceType;

	for (int i=0; i < keys.count(); i++)
	{
		if (keys[i] == device.name())
		{
			deviceName = values[i];
			deviceType = types[i];
			break;
		}
	}

	m_deviceName = device.name();
	ui->lbDevice->setText(device.name());
	ui->cbModel->setEditText(deviceName);
	ui->cbType->setEditText(deviceType);
}

void DeviceInformationWidget::deleteLater()
{
	QObject::deleteLater();
}

void DeviceInformationWidget::storeConfig()
{
	QStringList keys = SSCConfig::deviceNameKeys();
	QStringList values = SSCConfig::deviceNameValues();
	QStringList types = SSCConfig::deviceNameTypes();
	

	int keyIndex = keys.indexOf(m_deviceName);
	if (keyIndex == -1)
	{
		keyIndex = 0;
		keys.insert(0, m_deviceName);
		values.insert(0, "");
		types.insert(0, "");
	}

	values.replace(keyIndex, ui->cbModel->currentText());
	types.replace(keyIndex, ui->cbType->currentText());

	SSCConfig::setDeviceNameKeys(keys);
	SSCConfig::setDeviceNameValues(values);
	SSCConfig::setDeviceNameTypes(types);
}

bool DeviceInformationWidget::isComplete() const
{
	return (!ui->cbModel->currentText().isEmpty()) && (!ui->cbType->currentText().isEmpty());
}

DeviceInformationWidget::~DeviceInformationWidget()
{
	delete ui;
}

