#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>


namespace Ui {
    class MainWindow;
}

class QProgressBar;

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    MainWindow(QMainWindow *parent = 0);
    QProgressBar *ui_pb1;

protected:

private:
    Ui::MainWindow *ui;
    void keyPressEvent(QKeyEvent *e);

private slots:
    void inc_pb();
};

#endif // MAINWINDOW_H
