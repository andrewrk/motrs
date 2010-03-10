#ifndef PLAYTESTTHREAD_H
#define PLAYTESTTHREAD_H

#include <QThread>

class MainWindow;

class PlaytestThread : public QThread
{
    Q_OBJECT
public:
    PlaytestThread();

    void run();
    void endGameplay();
private:
    MainWindow * m_mainWindow;
};

#endif // PLAYTESTTHREAD_H
