#include "PlaytestThread.h"

#include <QApplication>
#include <QDir>

#include "Config.h"
#include "MainWindow.h"

#include "moc_PlaytestThread.cxx"

PlaytestThread::PlaytestThread() :
    m_mainWindow(NULL)
{
}

void PlaytestThread::run()
{
    Config::initialize(QDir(QApplication::applicationDirPath()).absoluteFilePath("config.ini").toStdString());

    m_mainWindow = new MainWindow();
    m_mainWindow->exec();

    delete m_mainWindow;
}

void PlaytestThread::endGameplay()
{
    m_mainWindow->close();
}
