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

#include "desktopgridcommandmanager.h"
#include <simonlogging/logger.h>
#include <simonactions/commandlistwidget.h>
#include <eventsimulation/eventhandler.h>
#include <KLocalizedString>
#include <KGenericFactory>
#include <KAction>
#include <KPushButton>
#include <QGridLayout>
#include <QDesktopWidget>
#include <QKeyEvent>
#include <QWidget>
#include "desktopgridconfiguration.h"

K_PLUGIN_FACTORY( DesktopGridPluginFactory,
registerPlugin< DesktopGridCommandManager >();
)

K_EXPORT_PLUGIN( DesktopGridPluginFactory("simondesktopgridcommand") )

DesktopGridCommandManager::DesktopGridCommandManager(QObject* parent, const QVariantList& args) :
CommandManager((Scenario*) parent, args),
GreedyReceiver(this),
screenGrid(new ScreenGrid(0)),
m_x(0),
m_y(0),
m_startX(0),
m_startY(0),
m_isDragging(false),
buttons(new QGridLayout(screenGrid)),
commandListWidget(new CommandListWidget()),
background(0)
{
}


const QString DesktopGridCommandManager::iconSrc() const
{
  return "games-config-board";
}


const QString DesktopGridCommandManager::preferredTrigger() const
{
  return "";
}


const QString DesktopGridCommandManager::name() const
{
  return i18n("Desktopgrid");
}


void DesktopGridCommandManager::activate()
{
  Logger::log(i18n("Activating desktopgrid"));
  startGreedy();
  init();
}


void DesktopGridCommandManager::deactivate()
{
  commandListWidget->hide();
  commandListWidget->abortTimeoutSelection();
  screenGrid->hide();
  stopGreedy();

  if (background) {
    background->deleteLater();
    background = 0;
  }
  switchToState(SimonCommand::DefaultState);
}


bool DesktopGridCommandManager::deSerializeConfig(const QDomElement& elem)
{
  config = new DesktopGridConfiguration(parentScenario);
  config->deSerialize(elem);

  KAction *activateAction = new KAction(this);
  activateAction->setText(i18n("Activate Desktopgrid"));
  activateAction->setStatusTip(i18n("Display the desktop grid"));
  activateAction->setIcon(KIcon("games-config-board"));
  connect(activateAction, SIGNAL(triggered(bool)),
    this, SLOT(activate()));
  guiActions << activateAction;

  connect(screenGrid, SIGNAL(cancel()), this, SLOT(deactivate()), Qt::DirectConnection);
  screenGrid->setContentsMargins(0,0,0,0);
  buttons->setSpacing(0);
  buttons->setMargin(0);

  QSize desksize = QDesktopWidget().screenGeometry().size();
  QBrush transbrush(QColor(241,241,241,100));
  short btnNr=1;
  for (int i=0; i < 3; i++) {
    for (int j=0; j<3; j++) {
      KPushButton *btn = new KPushButton(QString::number(btnNr), screenGrid);

      QPalette pal = btn->palette();
      pal.setBrush(QPalette::Button, transbrush);
      btn->setPalette(pal);

      btn->setMinimumHeight(desksize.height()/3);
      btn->setMinimumWidth(1);
      setButtonFontSize(btn);
      connect(btn, SIGNAL(clicked()), this, SLOT(regionSelected()), Qt::DirectConnection);
      buttons->addWidget(btn, i, j);
      btnNr++;
      btns << btn;
    }
  }

  buttons->setGeometry(screenGrid->geometry());
  screenGrid->setLayout(buttons);
  screenGrid->hide();

  commandListWidget->init(QStringList() << "input-mouse" << "input-mouse" <<
    "input-mouse" << "input-mouse" << "input-mouse",
    QStringList() << i18n("Left click") <<
    i18n("Double click") <<
    i18n("Right click") <<
    i18n("Middle click") <<
    i18n("Drag & Drop"), 0);                      //Add Elements for the list

  connect(commandListWidget, SIGNAL(runRequest(int)), this, SLOT(clickRequestReceived(int)), Qt::DirectConnection);
  connect(commandListWidget, SIGNAL(canceled()), this, SLOT(deactivate()), Qt::DirectConnection);

  if (!installInterfaceCommands()) {
    kDebug() << "Failed to install interface commands";
    return false;
  }

  return true;
}


bool DesktopGridCommandManager::installInterfaceCommands()
{
  bool succ = true;

  //launcher
  succ &= installInterfaceCommand(this, "activate", i18n("Desktopgrid"), iconSrc(),
    i18n("Starts the desktop grid"), true /* announce */, true /* show icon */,
    SimonCommand::DefaultState /* consider this command when in this state */,
    SimonCommand::GreedyState,                    /* if executed switch to this state */
    QString() /* take default visible id from action name */,
    "startDesktopGrid" /* id */);

  //number input
  succ &= installInterfaceCommand(btns[0], "click", i18nc("Number", "One"), iconSrc(),
  //	succ &= installInterfaceCommand(this, "clickTest", i18n("One"), iconSrc(),
    i18n("Clicks 1"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "1", "select1");
  succ &= installInterfaceCommand(btns[1], "click", i18nc("Number", "Two"), iconSrc(),
    i18n("Clicks 2"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "2", "select2");
  succ &= installInterfaceCommand(btns[2], "click", i18nc("Number", "Three"), iconSrc(),
    i18n("Clicks 3"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "3", "select3");
  succ &= installInterfaceCommand(btns[3], "click", i18nc("Number", "Four"), iconSrc(),
    i18n("Clicks 4"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "4", "select4");
  succ &= installInterfaceCommand(btns[4], "click", i18nc("Number", "Five"), iconSrc(),
    i18n("Clicks 5"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "5", "select5");
  succ &= installInterfaceCommand(btns[5], "click", i18nc("Number", "Six"), iconSrc(),
    i18n("Clicks 6"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "6", "select6");
  succ &= installInterfaceCommand(btns[6], "click", i18nc("Number", "Seven"), iconSrc(),
    i18n("Clicks 7"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "7", "select7");
  succ &= installInterfaceCommand(btns[7], "click", i18nc("Number", "Eight"), iconSrc(),
    i18n("Clicks 8"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "8", "select8");
  succ &= installInterfaceCommand(btns[8], "click", i18nc("Number", "Nine"), iconSrc(),
    i18n("Clicks 9"), false, false, SimonCommand::GreedyState,
    SimonCommand::GreedyState, "9", "select9");

  succ &= installInterfaceCommand(this, "deactivate", i18nc("Cancel the destkop grid", "Cancel"), "dialog-cancel",
    i18n("Aborts the selection process"), true, false, SimonCommand::GreedyState,
    SimonCommand::DefaultState, QString(), "cancel");

  succ &= installListInterfaceCommand(CommandListElements::One, this, "select1", "click1",
    i18n("In the click mode selection popup, selects the simple click with the left mouse button"),
    SimonCommand::GreedyState+1, SimonCommand::DefaultState);
  succ &= installListInterfaceCommand(CommandListElements::Two, this, "select2", "click2",
    i18n("In the click mode selection popup, selects the double click with the left mouse button"),
    SimonCommand::GreedyState+1, SimonCommand::DefaultState);
  succ &= installListInterfaceCommand(CommandListElements::Three, this, "select3", "click3",
    i18n("In the click mode selection popup, selects the simple click with the right mouse button"),
    SimonCommand::GreedyState+1, SimonCommand::DefaultState);
  succ &= installListInterfaceCommand(CommandListElements::Four, this, "select4", "click4",
    i18n("In the click mode selection popup, selects the simple click with the middle mouse button"),
    SimonCommand::GreedyState+1, SimonCommand::DefaultState);
  succ &= installListInterfaceCommand(CommandListElements::Five, this, "select5", "click5",
    i18n("In the click mode selection popup, this selects the drag and drop mode"),
    SimonCommand::GreedyState+1, SimonCommand::DefaultState);
  succ &= installListInterfaceCommand(CommandListElements::Cancel, this, "deactivate", "cancelClickModeSelection",
    i18n("In the click mode selection popup, cancels the selection"),
    SimonCommand::GreedyState+1, SimonCommand::DefaultState);

  return succ;
}


void DesktopGridCommandManager::init()
{
  m_isDragging = false;
  screenGrid->move(0,0);
  QSize deskSize = QDesktopWidget().screenGeometry().size();
  screenGrid->raise();

  buttons->setGeometry(screenGrid->geometry());

  foreach (KPushButton *btn, btns) {
    setButtonFontSize(btn);
    btn->setMinimumHeight(deskSize.height()/3);
    btn->setMinimumWidth(1);
    setButtonFontSize(btn);
  }

  screenGrid->setMaximumWidth(deskSize.width());
  screenGrid->setMaximumHeight(deskSize.height());
  screenGrid->setMinimumWidth(deskSize.width());
  screenGrid->setMinimumHeight(deskSize.height());

  screenGrid->resize(deskSize);

  if (!static_cast<DesktopGridConfiguration*>(config)->useRealTransparency()) {
    if (background) background->deleteLater();

    background = new QLabel(screenGrid);
    background->lower();
    deskShot = makeFakeTransparency();
    background->setPixmap(deskShot);
    background->move(0,0);
    background->resize(screenGrid->size());
  } else
  screenGrid->setWindowOpacity(0.55);

  //buttons->setGeometry(QRect(0, 0, screenGrid->geometry().width(), screenGrid->geometry().height()));

  screenGrid->show();
}


void DesktopGridCommandManager::setButtonFontSize(KPushButton *btn)
{
  QFont f = btn->font();
  f.setPointSize(btn->height()/4-2);
  btn->setFont(f);
}


QPixmap DesktopGridCommandManager::makeFakeTransparency()
{
  return QPixmap::grabWindow(QApplication::desktop()->winId()).copy(screenGrid->geometry());
}


void DesktopGridCommandManager::sendDragAndDrop()
{
  EventHandler::getInstance()->dragAndDrop(m_startX, m_startY, m_x,m_y);
}


void DesktopGridCommandManager::click(KPushButton* btn)
{
  m_x = screenGrid->x()+btn->x()+(btn->width()/2);
  m_y = screenGrid->y()+btn->y()+(btn->height()/2);
  screenGrid->hide();

  if (m_isDragging) {
    kDebug() << "Sending drag and drop";
    sendDragAndDrop();
    deactivate();
    return;
  }

  DesktopGridConfiguration::ActionSelection modeSelection = static_cast<DesktopGridConfiguration*>(config)->actionSelection();
  EventSimulation::ClickMode mode = static_cast<DesktopGridConfiguration*>(config)->clickMode();
  kDebug() << modeSelection << mode;

  switchToState(SimonCommand::GreedyState+1);     // switch to new state

  switch (modeSelection) {
    case DesktopGridConfiguration::AlwaysAsk:
      showSelectionBox();
      break;
    case DesktopGridConfiguration::UseDefault:
      clickRequestReceived((int) mode);
      break;
    case DesktopGridConfiguration::AskButDefaultAfterTimeout:
      showSelectionBox();
      commandListWidget->selectAfterTimeout((int) mode, static_cast<DesktopGridConfiguration*>(config)->askTimeout());
      break;
  }
}


void DesktopGridCommandManager::showSelectionBox()
{
  commandListWidget->adaptToVoiceElement(CommandListElements::One, getVoiceInterfaceCommand("click1"));
  commandListWidget->adaptToVoiceElement(CommandListElements::Two, getVoiceInterfaceCommand("click2"));
  commandListWidget->adaptToVoiceElement(CommandListElements::Three, getVoiceInterfaceCommand("click3"));
  commandListWidget->adaptToVoiceElement(CommandListElements::Four, getVoiceInterfaceCommand("click4"));
  commandListWidget->adaptToVoiceElement(CommandListElements::Five, getVoiceInterfaceCommand("click5"));
  commandListWidget->adaptToVoiceElement(CommandListElements::Cancel, getVoiceInterfaceCommand("cancelClickModeSelection"));

  commandListWidget->show();
}


void DesktopGridCommandManager::clickRequestReceived(int index)
{
  commandListWidget->hide();
  commandListWidget->abortTimeoutSelection();

  kDebug() << "Click request received: " << index;
  switch (index) {
    case 1: sendClick(EventSimulation::LMB);
    kDebug() << "LMB";
    break;
    case 2: sendClick(EventSimulation::LMBDouble);
    kDebug() << "LMBDouble";
    break;
    case 3: sendClick(EventSimulation::RMB);
    kDebug() << "RMB";
    break;
    case 4: sendClick(EventSimulation::MMB);
    kDebug() << "MMB";
    break;
    case 5:
      m_startX = m_x;
      m_startY = m_y;
      init();
      m_isDragging = true;
      switchToState(SimonCommand::GreedyState);
      return;                                     //do not deactivate me
  }
  deactivate();
}


void DesktopGridCommandManager::sendClick(EventSimulation::ClickMode clickMode)
{
  EventHandler::getInstance()->click(m_x,m_y, clickMode);
}


void DesktopGridCommandManager::regionSelected()
{
  KPushButton *senderBtn = dynamic_cast<KPushButton*>(sender());
  if (!senderBtn) return;

  if ((senderBtn->width() <= 20) && (senderBtn->height() <= 20)) {
    click(senderBtn);
    return;
  }
  QSize btnSize = senderBtn->size();
  QPoint pos = senderBtn->pos();

  pos.setX(pos.x()+screenGrid->x());
  pos.setY(pos.y()+screenGrid->y());

  int btnHeight = senderBtn->height()/3;

  foreach (KPushButton *btn, btns) {
    setButtonFontSize(btn);
    btn->setMinimumHeight(btnHeight);
  }

  screenGrid->setMinimumWidth(btnSize.width());
  screenGrid->setMaximumWidth(btnSize.width());

  screenGrid->setMinimumHeight(btnSize.height());
  screenGrid->setMaximumHeight(btnSize.height());

  screenGrid->move(pos);

  screenGrid->repaint();

  if (!static_cast<DesktopGridConfiguration*>(config)->useRealTransparency()) {
    background->resize(screenGrid->size());
    background->move(0,0);
    background->setPixmap(deskShot.copy(screenGrid->geometry()));
  }
}


bool DesktopGridCommandManager::deSerializeCommandsPrivate(const QDomElement& elem)
{
  Q_UNUSED(elem);

  return true;
}


DesktopGridCommandManager::~DesktopGridCommandManager()
{
  deactivate();
  commandListWidget->deleteLater();
  buttons->deleteLater();
  screenGrid->deleteLater();
}
