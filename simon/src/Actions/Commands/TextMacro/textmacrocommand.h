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


#ifndef TEXTMACRO_H
#define TEXTMACRO_H


#include "../command.h"


/**
 *	@class TextMacroCommand
 *	@brief Ressembles one tectmacro
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class TextMacroCommand : public Command {
Q_OBJECT
private:
	QString text;

protected:
	bool triggerPrivate();
	const QMap<QString,QVariant> getValueMapPrivate() const;

public:
	static const QString staticCategoryText();
	static const KIcon staticCategoryIcon();

	const KIcon getCategoryIcon() const;
	const QString getCategoryText() const;

	
    /**
    * @brief Constructor
    * 
    *	@author Peter Grasch
    */
    TextMacroCommand(const QString& name, const QString& iconSrc, const QString& text) : Command(name, iconSrc)
    {
        this->text = text;
    }

    /**
    * @brief Returns the text
    * 
    *	@author Peter Grasch
    */
    const QString getText() const { return this->text; }
    

	void change(const QString& newName, const QString& newIconSrc, const QString& newText) { 
		this->text = newText;
		Command::change(newName, newIconSrc);
	}
    
    ~TextMacroCommand() {}

};

#endif
