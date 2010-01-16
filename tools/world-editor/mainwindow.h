#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>

#include "worldview.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent * e);
    void showEvent(QShowEvent * e);

private:
    Ui::MainWindow *ui;

    WorldView * m_view;

private slots:
    void on_actionSettings_triggered();
};

#endif // MAINWINDOW_H
