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


#ifndef SIMON_VOICEINTERCEPTOR_H_D2600C6D435E4D26A75AC74A42A31C60
#define SIMON_VOICEINTERCEPTOR_H_D2600C6D435E4D26A75AC74A42A31C60

#include "simonvoiceui_export.h"
#include <QString>
#include <QDomElement>

class QObject;
class QDomDocument;

class SIMONVOICEUI_EXPORT VoiceInterceptor {
private:
	QString m_id;
	QString m_trigger;
	QString m_visibleTrigger;
	QString m_description;
	
public:
	VoiceInterceptor(const QString& id, const QString& trigger, const QString& visibleTrigger, 
			const QString& description);
	
	virtual ~VoiceInterceptor();

	bool deSerialize(const QDomElement&);
	QDomElement serialize(QDomDocument *doc);

	virtual bool trigger(const QString& text)=0;

};

#endif

