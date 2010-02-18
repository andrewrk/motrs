#include <QtGui/QApplication>
#include <QtCore/QDir>
#include "WorldEditor.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // set these values so we can use default settings constructor
    QCoreApplication::setOrganizationName("Motrs INC");
    QCoreApplication::setOrganizationDomain("motrs.superjoesoftware.com");
    QCoreApplication::setApplicationName("Motrs Game Editor");

    // move updater.exe.new to updater.exe
    QDir appDir(QApplication::applicationDirPath());
    QFile updater(appDir.absoluteFilePath("updater.exe.new"));
    if( updater.exists() ) {
        // delete updater.exe
        appDir.remove("updater.exe");
        // rename updater.exe.new to updater.exe
        updater.rename("updater.exe");
    }

    WorldEditor w;
    w.show();
    return a.exec();
}
