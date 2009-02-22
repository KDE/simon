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

#ifndef TRAINSAMPLEPAGE_H
#define TRAINSAMPLEPAGE_H

#include <QString>
#include <QWizardPage>
class RecWidget; 

class TrainSamplePage : public QWizardPage
{
	Q_OBJECT
	
	private:
		RecWidget *recorder;
		QString prompt;
		QString fileName;
	
	public:
		TrainSamplePage(const QString& prompt, int nowPage, int maxPage, const QString name, QWidget *parent=0);
		~TrainSamplePage();
		bool isComplete() const;

		void initializePage();
		
		QString getPrompt() { return prompt; }
		QString getFileName() { return fileName; }
		
	public slots:
		bool submit();
		bool cleanUp();

};

#endif
