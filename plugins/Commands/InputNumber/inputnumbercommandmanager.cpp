/*
 *   Copyright (C) 2008 Peter Grasch <grasch@simon-inputnumberens.org>
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
#include "inputnumbercommandmanager.h"
#include <eventsimulation/eventhandler.h>
#include <simonactions/actionmanager.h>

//sleep function
#ifdef Q_OS_WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include <QDesktopWidget>
#include <KDialog>
#include <KLocalizedString>
#include <KAction>
#include <KPluginFactory>
#include <KDebug>

K_PLUGIN_FACTORY( InputNumberCommandPluginFactory,
registerPlugin< InputNumberCommandManager >();
)

K_EXPORT_PLUGIN( InputNumberCommandPluginFactory("simoninputnumbercommand") )

InputNumberCommandManager::InputNumberCommandManager(QObject* parent, const QVariantList& args) : CommandManager((Scenario*) parent, args),
GreedyReceiver(this),
widget(0),
activateAction(0)
{

}


bool InputNumberCommandManager::deSerializeConfig(const QDomElement& elem)
{
  activateAction = new KAction(this);
  activateAction->setStatusTip(i18n("Display the number input"));
  widget = new KDialog(0, Qt::Dialog|Qt::WindowStaysOnTopHint);
  Q_UNUSED(elem);
  widget->setWindowIcon(KIcon("accessories-calculator"));
  setFont(ActionManager::getInstance()->pluginBaseFont());
  connect(widget, SIGNAL(rejected()), this, SLOT(deregister()));

  QWidget *internalWidget = new QWidget(widget);
  ui.setupUi(internalWidget);
  widget->setMainWidget(internalWidget);
  widget->setButtons(0); //don't show any kdialog buttons

  ui.pbOk->setIcon(KIcon("dialog-ok-apply"));
  ui.pbCancel->setIcon(KIcon("dialog-cancel"));
  widget->hide();

  activateAction->setText(i18n("Activate Number Input"));
  activateAction->setIcon(KIcon("accessories-calculator"));
  connect(activateAction, SIGNAL(triggered(bool)),
    this, SLOT(activate()));
  guiActions << activateAction;

  connect(widget, SIGNAL(finished(int)), this, SLOT(deregister()));
  connect(ui.pbCancel, SIGNAL(clicked()), this, SLOT(cancel()));
  connect(ui.pbBack, SIGNAL(clicked()), this, SLOT(back()));
  connect(ui.pbOk, SIGNAL(clicked()), this, SLOT(ok()));
  connect(ui.pbComma, SIGNAL(clicked()), this, SLOT(sendComma()));
  connect(ui.pb0, SIGNAL(clicked()), this, SLOT(send0()));
  connect(ui.pb1, SIGNAL(clicked()), this, SLOT(send1()));
  connect(ui.pb2, SIGNAL(clicked()), this, SLOT(send2()));
  connect(ui.pb3, SIGNAL(clicked()), this, SLOT(send3()));
  connect(ui.pb4, SIGNAL(clicked()), this, SLOT(send4()));
  connect(ui.pb5, SIGNAL(clicked()), this, SLOT(send5()));
  connect(ui.pb6, SIGNAL(clicked()), this, SLOT(send6()));
  connect(ui.pb7, SIGNAL(clicked()), this, SLOT(send7()));
  connect(ui.pb8, SIGNAL(clicked()), this, SLOT(send8()));
  connect(ui.pb9, SIGNAL(clicked()), this, SLOT(send9()));

  kDebug() << "Installed commands!";
  bool succ = true;
  succ &= installInterfaceCommand(this, "activate", i18n("Number"), iconSrc(),
    i18n("Displays the number input field"), true /* announce */, true /* show icon */,
    SimonCommand::DefaultState /* consider this command when in this state */,
    SimonCommand::GreedyState,                    /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "startInputNumber" /* id */);

  succ &= installInterfaceCommand(ui.pb0, "click", i18nc("Number", "Zero"), iconSrc(),
    i18n("Clicks 0"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "0");
  succ &= installInterfaceCommand(ui.pb1, "click", i18nc("Number", "One"), iconSrc(),
    i18n("Clicks 1"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "1");
  succ &= installInterfaceCommand(ui.pb2, "click", i18nc("Number", "Two"), iconSrc(),
    i18n("Clicks 2"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "2");
  succ &= installInterfaceCommand(ui.pb3, "click", i18nc("Number", "Three"), iconSrc(),
    i18n("Clicks 3"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "3");
  succ &= installInterfaceCommand(ui.pb4, "click", i18nc("Number", "Four"), iconSrc(),
    i18n("Clicks 4"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "4");
  succ &= installInterfaceCommand(ui.pb5, "click", i18nc("Number", "Five"), iconSrc(),
    i18n("Clicks 5"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "5");
  succ &= installInterfaceCommand(ui.pb6, "click", i18nc("Number", "Six"), iconSrc(),
    i18n("Clicks 6"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "6");
  succ &= installInterfaceCommand(ui.pb7, "click", i18nc("Number", "Seven"), iconSrc(),
    i18n("Clicks 7"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "7");
  succ &= installInterfaceCommand(ui.pb8, "click", i18nc("Number", "Eight"), iconSrc(),
    i18n("Clicks 8"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "8");
  succ &= installInterfaceCommand(ui.pb9, "click", i18nc("Number", "Nine"), iconSrc(),
    i18n("Clicks 9"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "9");
  succ &= installInterfaceCommand(ui.pbComma, "click", i18nc("Decimal separator (voice trigger)", "Point"), iconSrc(),
    i18n("Decimal separator"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, KGlobal::locale()->decimalSymbol());

  succ &= installInterfaceCommand(ui.pbBack, "click", i18nc("Word to say to remove the last recognized digit", "Back"), iconSrc(),
    i18n("Deletes the last input character"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState);
  succ &= installInterfaceCommand(ui.pbOk, "click", i18n("OK"), "dialog-ok-apply",
    i18n("Accepts the input dialog and writes the number"), false, true, SimonCommand::GreedyState,
    SimonCommand::GreedyState);
  succ &= installInterfaceCommand(ui.pbCancel, "click", i18n("Cancel"), "dialog-cancel",
    i18n("Aborts the input dialog"), false, true, SimonCommand::GreedyState,
    SimonCommand::GreedyState);
  return succ;
}


const QString InputNumberCommandManager::preferredTrigger() const
{
  return "";
}


void InputNumberCommandManager::setFont(const QFont& font)
{
  widget->setFont(font);
}


void InputNumberCommandManager::activate()
{
  ui.leNumber->clear();
  QDesktopWidget* tmp = QApplication::desktop();
  int x,y;
  x=(tmp->width()/2) - (widget->width()/2);
  y=(tmp->height()/2)-(widget->height()/2);
  widget->move(x, y);
  widget->show();
  startGreedy();
}


void InputNumberCommandManager::deregister()
{
  stopGreedy();
}


const QString InputNumberCommandManager::iconSrc() const
{
  return ("accessories-calculator");
}


const QString InputNumberCommandManager::name() const
{
  return i18n("Input Number");
}


void InputNumberCommandManager::sendComma()
{
  ui.leNumber->setText(ui.leNumber->text()+i18nc("Decimal point", "."));
}


void InputNumberCommandManager::back()
{
  QString text = ui.leNumber->text();
  if (text.count() == 0) return;

  text = text.left(text.count()-1);
  ui.leNumber->setText(text);
}


void InputNumberCommandManager::cancel()
{
  widget->reject();
}


void InputNumberCommandManager::processRequest(int number)
{
  ui.leNumber->setText(ui.leNumber->text()+QString::number(number));
}


void InputNumberCommandManager::ok()
{
  widget->accept();
  #ifdef Q_OS_WIN32
  Sleep(300);
  #else
  usleep(300000);
  #endif
  EventHandler::getInstance()->sendWord(ui.leNumber->text());
}


InputNumberCommandManager::~InputNumberCommandManager()
{
  widget->deleteLater();
}
