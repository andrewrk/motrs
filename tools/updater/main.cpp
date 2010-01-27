#include <QtGui/QApplication>
#include "UpdaterDialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set these values so we can use default settings constructor
    QCoreApplication::setOrganizationName("Motrs INC");
    QCoreApplication::setOrganizationDomain("motrs.superjoesoftware.com");
    QCoreApplication::setApplicationName("Motrs Updater");

    UpdaterDialog w;
    w.show();
    return a.exec();
}
