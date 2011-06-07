/*
 *   Copyright (C) 2011 Adam Nash <adam.t.nash@gmail.com>
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

#ifndef SIMON_CREATECONDITIONWIDGET_H_953FB9DDF3FA4DC5965BF6C9797CAA8A
#define SIMON_CREATECONDITIONWIDGET_H_953FB9DDF3FA4DC5965BF6C9797CAA8A

#include "simoncontextdetection_export.h"

#include <QWidget>

class Condition;
class CompoundCondition;
class KIcon;

/**
 *	@class CreateConditionWidget
 *	@brief Provides an abstract base-class for the widget that is going to be displayed in the New/Modify-Condition-Dialog
 *
 *	If you want your conditions to be user configurable, you most likely want to derive a class from CreateConditionWidget.
 *	CreateConditionWidget are the graphical "factories" for Conditions where users can define and edit their conditions.
 *
 *	To enable the user to add and edit your Condition subclass, subclass this class and return an instance of it in the
 *	Condition::getCreateCommandWidget() method.
 */
class SIMONCONTEXTDETECTION_EXPORT CreateConditionWidget : public QWidget
{
  Q_OBJECT

    signals:
  /**
   * \brief The dialog should re-check if the condition was completely specified
   * \sa isComplete();
   */
  void completeChanged();

  protected:
    /// The parent compound condition
    CompoundCondition *m_compoundCondition;

  protected slots:


  public:
    /**
     * @brief Creates the condition
     * @author Adam Nash
     *
     * This function gets the common parameters for all conditions (name, iconSrc and description).
     *
     * A plugin implementing this should add the additional parameters of its Condition-derived
     * class (like the name of the program that it is monitoring) and create an instance
     * of the derived class.
     *
     * If you do not have enough information to create the condition (i.e. the user didn't
     * enter all the required fields), return a nullpointer.
     *
    */
    virtual Condition* createCondition()=0;

    /**
     * \brief Creates the condition and adds it to the parent compound condition
     * Calls createCondition() and adds the result to the manager by calling CompoundCondition::addCondition().
     * \sa createCondition()
     */
    virtual bool addCondition();

    /**
     * \brief When editing a condition this method will be called to initialize the display with the old values
     *
     * This method should adapt the current configuration to the values of the given condition.
     *
     * \param condition The condition to display
     * \return False, if for some reason the input fields could not be set to the values configured in the given condition
     */
    virtual bool init(Condition* condition)=0;

    /**
     * \brief Have all needed attributes been specified?
     * \return True if all necessary fields have been filled out
     */
    virtual bool isComplete()=0;

    explicit CreateConditionWidget(CompoundCondition *compoundCondition, QWidget *parent=0) : QWidget(parent),
      m_compoundCondition(compoundCondition)
      {}

    virtual ~CreateConditionWidget() {}
};
#endif

