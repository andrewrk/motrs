#ifndef WORLDVIEW_H
#define WORLDVIEW_H

#include <QWidget>
#include <QScrollBar>
#include <QPaintEvent>
#include <QResizeEvent>

class WorldView : public QWidget
{
    Q_OBJECT
public:
    WorldView(QWidget * parent = NULL);
    ~WorldView();
protected:
    void resizeEvent(QResizeEvent * e);
    void paintEvent(QPaintEvent * e);
private:
    QScrollBar * m_hsb;
    QScrollBar * m_vsb;
};

#endif // WORLDVIEW_H
