#ifndef EDITORGRAPHICTIMER_H
#define EDITORGRAPHICTIMER_H

#include <QTimer>

// simply calls the updateFrameCount() static method in EditorGraphic on a timer
class EditorGraphicTimer : public QObject
{
    Q_OBJECT
public:
    EditorGraphicTimer(int msec);
private:
    QTimer m_timer;
private slots:
    void timeout();
};

#endif // EDITORGRAPHICTIMER_H
