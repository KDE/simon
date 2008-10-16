/*
 * simond.h
 *
 * Copyright (C) 2008 Peter Grasch <grasch@simon-listens.org>
 */
#ifndef SIMOND_H
#define SIMOND_H


#include <kxmlguiwindow.h>

#include "ui_prefs_base.h"

class simondView;
class QPrinter;
class KToggleAction;
class KUrl;

/**
 * This class serves as the main window for simond.  It handles the
 * menus, toolbars, and status bars.
 *
 * @short Main window class
 * @author Peter Grasch <grasch@simon-listens.org>
 * @version 0.1
 */
class simond : public KXmlGuiWindow
{
    Q_OBJECT
public:
    /**
     * Default Constructor
     */
    simond();

    /**
     * Default Destructor
     */
    virtual ~simond();

private slots:
    void fileNew();
    void optionsPreferences();

private:
    void setupActions();

private:
    Ui::prefs_base ui_prefs_base ;
    simondView *m_view;

    QPrinter   *m_printer;
    KToggleAction *m_toolbarAction;
    KToggleAction *m_statusbarAction;
};

#endif // _SIMOND_H_
