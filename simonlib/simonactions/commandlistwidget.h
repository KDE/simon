/*
 *   Copyright (C) 2008 Peter Grasch <peter.grasch@bedahr.org>
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

#ifndef SIMON_COMMANDLISTWIDGET_H_055CB08189FE40A4B28221757CDEB148
#define SIMON_COMMANDLISTWIDGET_H_055CB08189FE40A4B28221757CDEB148

#include "simonactions_export.h"
#include <simonscenarios/command.h>
#include <simonscenarios/commandlistelements.h>
#include <QTimer>
#include <QWidget>

class QCloseEvent;
class QPushButton;
class QTableWidget;
class QProgressBar;
class QShowEvent;
class VoiceInterfaceCommand;

/**
 * \class CommandListWidget
 * @brief Provides a graphical list widget for the user to select from an array of options
 *
 * Use this class if you want to show the user an array of options. You can easily use
 * voice interface commands to control it if you use the installListInterfaceCommand()
 * method (see the \ref CommandManager documentation for more details).
 *
 * The CommandListWidget is the low level graphical widget that the ListCommand uses. Use
 * the ListCommand if you only need high level access but use this class directly if you
 * want a little bit more control (you most likely want to use the CommandListWidget directly;
 * The added benefit of using the ListCommands loose command binding only make sense in very
 * rare cases).
 *
 *
 * To use the CommandListWidget in your class, just create an object, initialize it with
 * the options you want it to present and connect the signal / slots:
 *
 * \code
commandListWidget = new CommandListWidget();
commandListWidget->init(QStringList() << "icon1" << "icon2",
QStringList() << i18n("Option 1") << i18n("Option 2"), 0);
connect(commandListWidget, SIGNAL(runRequest(int)), this, SLOT(writeoutRequestReceived(int)));
connect(commandListWidget, SIGNAL(canceled()), this, SLOT(cancel()));
\endcode
*
* This will create a list with numbered options (in this case "Option 1" and "Option 2").
* When the user clicks any of these options, the index will be emitted through \ref runRequest() and when
* the user selects cancel or closes the window, the \ref canceled() signal is emitted.
*
* To access the options with your voice you should implement something like the following in the
* deSerializeConfig method of your command manager subclass:
*
* \code
installListInterfaceCommand(CommandListElements::One, this, "selectOption1", "option1",
i18n("In the selection popup, selects the first option"));
installListInterfaceCommand(CommandListElements::One, this, "selectOption2", "option2",
i18n("In the selection popup, selects the second option"));
succ &= installListInterfaceCommand(CommandListElements::Cancel, this, "cancel", "selectCancel",
i18n("In the selection popup, selects cancel"));
\endcode
*
* And the slots could look something like this:
*
* \code
void selectOption1() { writeoutRequestReceived(1); }
void selectOption2() { writeoutRequestReceived(2); }
void cancel()
{
commandListWidget->hide();
commandListWidget->abortTimeoutSelection();
//...
}
\endcode
*
* This would ensure that you could select options / cancel with both your voice and by using
* the graphical widget.
*
* To actually show the command list widget you can just call show() but you should take into account
* that the user might have changed the voice interface commands. This is done by getting the current
* version of the voice interface elements and calling \ref adaptToVoiceElement. This will also
* automatically hide all elements that have no voice commands associated with them letting the user
* customize the functionality of the lists themselves (hide unwanted options for example). Hidden
* options will not effect the exported indizes (\ref runRequest()).
 *
 * \code
commandListWidget->adaptToVoiceElement(CommandListElements::One, getVoiceInterfaceCommand("option1"));
commandListWidget->adaptToVoiceElement(CommandListElements::Two, getVoiceInterfaceCommand("option1"));
commandListWidget->adaptToVoiceElement(CommandListElements::Cancel, getVoiceInterfaceCommand("selectCancel"));
commandListWidget->show();
\endcode
*
* The CommandListWidget provides a way to let the system automatically select one of the options after
* a timeout.
*
* The list will display eight options at most at a time. If you have more than that, the list will enable
* back / next buttons.
*/
class SIMONACTIONS_EXPORT CommandListWidget : public QWidget
{
  Q_OBJECT

    signals:
  /// \brief The option specified by the given index was selected
  /// \param index The index of the selected option (index of the list provided to \ref init())
  void runRequest(int index);

  /// Cancel was selected
  void canceled();

  private slots:
    /**
     * \brief Runs the option that is currently selected. Emits \ref runRequest().
     */
    void runCommand();

    /**
     * \brief Toggle / untoggle the \ref indexToSelectAfterTimeout to show the impending selection after timeout
     */
    void blink();

    /**
     * \brief The timeout was reached, we should execute the option referenced by \ref indexToSelectAfterTimeout
     */
    void timeoutReached();

  protected:
    /// The cancel button
    QPushButton *pbCancel;
    /// Table widget containing all the options
    QTableWidget *twCommands;
    /// Progress bar to show the approaching timeout if automatic selection after timeout is enabled
    QProgressBar *pbAutomaticSelection;

    /// Overwritten close event to make sure that \ref canceled() gets emitted
    void closeEvent(QCloseEvent *);

    /// Last minute GUI initialization
    void showEvent(QShowEvent *);

  public:
    /**
     * \enum Flag
     *
     * \brief Flags describing the options of the current command list widget
     */
    enum Flag
    {
      /// Do we have a back button?
      HasBack=0x1,
      /// Do we have a next button?
      HasNext=0x2
    };

    Q_DECLARE_FLAGS(Flags, Flag)

    /**
     * @brief Constructor
     */
      CommandListWidget();

    /**
     * \brief Initializes the widget with the given parameters
     *
     * This will create the graphical widget with the given options. Make sure that
     * both the \q iconsrcs and \q commands list have the same amount of items or
     * you will trigger an assert
     *
     * \param iconsrcs Icon names of the options
     * \param commands The action names of the associated commands (some descriptive title)
     * \param flags The flags of this command
     * \sa Flag
     */
    void init(const QStringList& iconsrcs, const QStringList commands, Flags flags);

    /**
     * \brief Call this method to let the list widget know when you have executed
     * 	  One of the options of the command list widget
     */
    void runRequestSent();

    /**
     * \brief Selects an index that should be run after the given timeout and starts the clock
     * \param index The index to run
     * \param timeout The amount of time (in milliseconds) to wait before executing the index
     */
    void selectAfterTimeout(int index, int timeout /* in ms */);

    /**
     * \brief Abort the selection of #indexToSelectAfterTimeout after a timeout
     */
    void abortTimeoutSelection();

    /**
     * \brief Sets the plugin base font to the given font
     * \param font The font to set the plugin to
     */
    void setFont(const QFont& font);

    /**
     * \brief Resizes the graphical widget to fit all the options
     */
    void resizeToFit();

    /**
     * \brief Adapt the given element to the provided voice interface command
     *
     * Call this method shortly before you show the wizard if you use voice interface commands
     * to interact with the command list widget.
     *
     * This will adapt the display to show the associated trigger (and icons) to the one the
     * voice interface command actually listens to.
     *
     * If you provide a null pointer as command, the element will be hidden.
     *
     * \param element The element of the command list widget to adapt
     * \param command The command to adjust to. 0 if you want to hide the element
     */
    void adaptToVoiceElement(CommandListElements::Element element, VoiceInterfaceCommand* command);

    /**
     * \brief Destructor
     */
    virtual ~CommandListWidget();

  private:
    /// Flags of the list widget (do we have back and / or next buttons?)
    Flags currentFlags;

    /// \brief Flag to remember if we emitted a run request
    /// Flag to remember if we emitted a run request to avoid emitting \ref canceled() when the dialog
    /// closes after we did run an option.
    bool runRequestEmitted;

    /// Countdown timer to select an option after a timeout
    QTimer toggleAfterTimeoutTimer;

    /// Calls \ref selectAfterTimeoutIndex() periodically
    QTimer blinkTimer;

    /// The index that should be selected after the timeout was reached
    int indexToSelectAfterTimeout;

    /// \brief Toggles / untoggles the option that should be selected after a timeout
    /// \param select Should the option be selected?
    void selectAfterTimeoutIndex(bool select);
};

Q_DECLARE_OPERATORS_FOR_FLAGS(CommandListWidget::Flags)
#endif
