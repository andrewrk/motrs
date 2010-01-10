#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setCentralWidget(ui->view);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::resizeEvent(QResizeEvent * e)
{

}

void MainWindow::showEvent(QShowEvent * e)
{
    // load universe

}

void MainWindow::on_actionSettings_triggered()
{
    if( SettingsDialog::instance()->exec() == QDialog::Accepted ) {

    }
}

#include "moc_mainwindow.cxx"
