#include <QtGui/QApplication>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set these values so we can use default settings constructor
    QCoreApplication::setOrganizationName("Motrs INC");
    QCoreApplication::setOrganizationDomain("motrs.superjoesoftware.com");
    QCoreApplication::setApplicationName("Motrs World Editor");

    MainWindow w;
    w.show();
    return a.exec();
}
