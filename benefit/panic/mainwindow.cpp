#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtGui>
#include <QProgressBar>

MainWindow::MainWindow(QMainWindow *parent) :
    QMainWindow(parent), ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Center the Window
    QDesktopWidget *desktop = QApplication::desktop();
    int screenWidth, width;
    int screenHeight, height;
    int x, y;
    QSize windowSize;
    screenWidth = desktop->width(); // get width of screen
    screenHeight = desktop->height(); // get height of screen
    windowSize = size(); // size of our application window
    width = windowSize.width();
    height = windowSize.height();
    // little computations
    x = (screenWidth - width) / 2;
    y = (screenHeight - height) / 2;
    y -= 50;
    // move window to desired coordinates
    move ( x, y );

    ui_pb1 = qFindChild<QProgressBar*>(this, "pb1");

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(inc_pb()));
    //connect(timer, SIGNAL(timeout()), this, SLOT(ttest(ui)));

    timer->start(1000);
}

void MainWindow::inc_pb()
{
    ui_pb1->setValue(ui_pb1->value()+1);
    if (ui_pb1->value() == ui_pb1->maximum())
    {
        system("sudo /sbin/reboot");
    }
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
    if(e->key() == Qt::Key_Q)
    {
        qApp->quit();
    }
}
