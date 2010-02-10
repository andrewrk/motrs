#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include <QWidget>
#include <QPaintEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>

class ObjectEditor;

class ObjectView : public QWidget
{
public:
    ObjectView();
protected:
    void paintEvent(QPaintEvent * e);
    void dropEvent(QDropEvent * e);
    void dragEnterEvent(QDragEnterEvent * e);
    void dragMoveEvent(QDragMoveEvent * e);
    void dragLeaveEvent(QDragLeaveEvent * e);
private:
    // contains a pixmap to draw when dragging art and such
    QPixmap * m_dragPixmap;
    int m_dragPixmapX;
    int m_dragPixmapY;
    ObjectEditor * m_window;
};

#endif // OBJECTVIEW_H
