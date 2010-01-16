#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "settingsdialog.h"


#include "moc_mainwindow.cxx"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_view(new WorldView(this))
{
    ui->setupUi(this);

    setCentralWidget(m_view);

    ui->dock_art->setWidget(ui->widget_art);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_view;
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
    SettingsDialog::instance()->exec();
}

