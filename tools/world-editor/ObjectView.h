#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include "EditorMap.h"

#include <QWidget>
#include <QPaintEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QPushButton>
#include <QScrollBar>

class ObjectEditor;

class ObjectView : public QWidget
{
    Q_OBJECT
public:
    enum ViewMode {
        Normal,
        Shape,
        SurfaceType,
    };

    ObjectView(ObjectEditor * window, QWidget * parent = NULL);
    ~ObjectView();

    void setSelectedLayer(int layer);
    void setViewMode(ViewMode mode);
    void open(QString file);

protected:
    void paintEvent(QPaintEvent * e);
    void resizeEvent(QResizeEvent * e);

    void dropEvent(QDropEvent * e);
    void dragEnterEvent(QDragEnterEvent * e);
    void dragMoveEvent(QDragMoveEvent * e);
    void dragLeaveEvent(QDragLeaveEvent * e);

private:

    // the host window for this view
    ObjectEditor * m_window;

    // the object we are editing
    EditorMap * m_object;

    // contains a pixmap to draw when dragging art and such
    QPixmap * m_dragPixmap;
    int m_dragPixmapX;
    int m_dragPixmapY;

    // gui elements on the edges of the view
    QPushButton * m_btnTopPlus;
    QPushButton * m_btnTopMinus;
    QPushButton * m_btnBottomPlus;
    QPushButton * m_btnBottomMinus;

    QPushButton * m_btnLeftPlus;
    QPushButton * m_btnLeftMinus;
    QPushButton * m_btnRightPlus;
    QPushButton * m_btnRightMinus;

    QScrollBar * m_hsb;
    QScrollBar * m_vsb;

    // must be kept in sync with the layers list widget
    int m_selectedLayer;

    ViewMode m_viewMode;

    // where is the editor scrolled to, in absolute coordinates
    double m_zoom;
    double m_offsetX;
    double m_offsetY;

    void refreshLayersList();

    double screenX(double absoluteX);
    double screenY(double absoluteY);
    double absoluteX(double screenX);
    double absoluteY(double screenY);

    void drawGrid(QPainter &p);

    void setUpScrolling();
private slots:
    void on_btnLeftPlus_clicked();
    void on_btnLeftMinus_clicked();
    void on_btnRightPlus_clicked();
    void on_btnRightMinus_clicked();
    void on_btnTopPlus_clicked();
    void on_btnTopMinus_clicked();
    void on_btnBottomPlus_clicked();
    void on_btnBottomMinus_clicked();

    void verticalScroll(int value);
    void horizontalScroll(int value);
};

#endif // OBJECTVIEW_H
