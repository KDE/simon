/*
 * simondview.h
 *
 * Copyright (C) 2007 Peter Grasch <grasch@simon-listens.org>
 */
#ifndef SIMONDVIEW_H
#define SIMONDVIEW_H

#include <QtGui/QWidget>

#include "ui_simondview_base.h"

class QPainter;
class KUrl;

/**
 * This is the main view class for simond.  Most of the non-menu,
 * non-toolbar, and non-statusbar (e.g., non frame) GUI code should go
 * here.
 *
 * @short Main view
 * @author Peter Grasch <grasch@simon-listens.org>
 * @version 0.1
 */

class simondView : public QWidget, public Ui::simondview_base
{
    Q_OBJECT
public:
    /**
     * Default constructor
     */
    simondView(QWidget *parent);

    /**
     * Destructor
     */
    virtual ~simondView();

private:
    Ui::simondview_base ui_simondview_base;

signals:
    /**
     * Use this signal to change the content of the statusbar
     */
    void signalChangeStatusbar(const QString& text);

    /**
     * Use this signal to change the content of the caption
     */
    void signalChangeCaption(const QString& text);

private slots:
    void switchColors();
    void settingsChanged();
};

#endif // simondVIEW_H
