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

#ifndef SIMON_DESKTOPGRIDCOMMANDMANAGER_H_A9E36FBF49D04CC0BCAF236731F8316B
#define SIMON_DESKTOPGRIDCOMMANDMANAGER_H_A9E36FBF49D04CC0BCAF236731F8316B

#include "screengrid.h"
#include <simonscenarios/commandmanager.h>
#include <simonactions/greedyreceiver.h>
#include <eventsimulation/clickmode.h>
#include <QVariantList>

class KPushButton;
class QGridLayout;
class QLabel;
class QWidget;
class DesktopGridConfiguration;
class CommandListWidget;

/**
 *	@class DesktopGridCommandManager
 *	@brief Manager of the ScreenGrid
 *
 *	@version 0.1
 *	@date 20.05.2008
 *	@author Peter Grasch
 */
class DesktopGridCommandManager : public CommandManager, public GreedyReceiver
{
  Q_OBJECT

    private:
    ScreenGrid *screenGrid;

    int m_x;
    int m_y;
    int m_startX;
    int m_startY;
    bool m_isDragging;
    QList<KPushButton*> btns;
    QGridLayout *buttons;

    CommandListWidget *commandListWidget;

    QLabel *background;
    QPixmap deskShot;

    QPixmap makeFakeTransparency();

    void click(KPushButton* btn);

    void init();

    void sendClick(EventSimulation::ClickMode clickMode);
    void sendDragAndDrop();

    bool installInterfaceCommands();

    void showSelectionBox();

  private slots:
    void regionSelected();
    void clickRequestReceived(int index);

    void select1() { clickRequestReceived(1); }
    void select2() { clickRequestReceived(2); }
    void select3() { clickRequestReceived(3); }
    void select4() { clickRequestReceived(4); }
    void select5() { clickRequestReceived(5); }

  public slots:
    void activate();
    void deactivate();

  public:
    const QString preferredTrigger() const;
    const QString name() const;

    bool deSerializeConfig(const QDomElement& elem);
    bool deSerializeCommandsPrivate(const QDomElement& elem);

    const QString iconSrc() const;

    void setButtonFontSize(KPushButton *btn);

    /**
     * @brief Constructor
     *
     *	@author Peter Grasch
     */
    DesktopGridCommandManager(QObject* parent, const QVariantList& args);

    ~DesktopGridCommandManager();

};
#endif
