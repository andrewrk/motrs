#ifndef OBJECTVIEW_H
#define OBJECTVIEW_H

#include "EditorObject.h"

#include <QWidget>
#include <QPaintEvent>
#include <QDropEvent>
#include <QDragEnterEvent>
#include <QDragMoveEvent>
#include <QDragLeaveEvent>
#include <QPushButton>
#include <QScrollBar>
#include <QTimer>

class ObjectEditor;

class ObjectView : public QWidget
{
    Q_OBJECT
public: //variables
    enum ViewMode {
        vmNormal,
        vmShape,
        vmSurfaceType,
    };

public: //methods
    ObjectView(ObjectEditor * window, QWidget * parent = NULL);
    ~ObjectView();

    void createEmpty();
    void open(QString file);

    void setSelectedLayer(int layer);
    void setViewMode(ViewMode mode);

    void setZoom(double zoom);
    void zoomIn();
    void zoomOut();

    // layer operations which operate on the selected layer
    void addLayer();
    void deleteLayer(int index);
    void swapLayers(int i, int j);

    // make sure all the controls and things are correct
    void refreshGui();

    void propertyChanged(int row);

    void deleteSelection();
    void cutSelection();
    void copySelection();
    void pasteSelection();

    // save the object
    void saveObject();

protected:
    void paintEvent(QPaintEvent * e);
    void resizeEvent(QResizeEvent * e);

    void mousePressEvent(QMouseEvent * e);
    void mouseMoveEvent(QMouseEvent * e);
    void mouseReleaseEvent(QMouseEvent * e);
    void wheelEvent(QWheelEvent * e);

    void dropEvent(QDropEvent * e);
    void dragEnterEvent(QDragEnterEvent * e);
    void dragMoveEvent(QDragMoveEvent * e);
    void dragLeaveEvent(QDragLeaveEvent * e);

private: //variables
    enum MouseState {
        msNormal,
        msStretchGraphicLeft,
        msStretchGraphicTop,
        msStretchGraphicRight,
        msStretchGraphicBottom,
        msMoveGraphic,
    };

    enum TableProperty {
        Name,
        Width,
        Height,
        Description,
    };

    static const char * c_surfaceTypes[];
    static const char * c_shapes[];

    // the host window for this view
    ObjectEditor * m_window;

    // the object we are editing
    EditorObject * m_object;

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

    // pixmaps used to display surface types and shapes
    static QList<QPixmap *> s_surfaceTypePixmaps;
    static QList<QPixmap *> s_shapePixmaps;
    static QStringList s_surfaceTypeNames;
    static QStringList s_shapeNames;

    EditorObject::ObjectGraphic * m_selectedGraphic;

    // cache the location of the mouse on mouse move
    int m_mouseX, m_mouseY;
    int m_mouseDownX, m_mouseDownY;
    MouseState m_mouseState;
    double m_startX, m_startY;
    double m_startWidth, m_startHeight;

    EditorObject::ObjectGraphic m_copyBuffer;

    QTimer m_animationTimer;

private: //methods

    int screenX(double absoluteX);
    int screenY(double absoluteY);
    double absoluteX(int screenX);
    double absoluteY(int screenY);

    int snapScreenX(int x);
    int snapScreenY(int y);
    double snapAbsoluteX(double x);
    double snapAbsoluteY(double y);

    void drawGrid(QPainter &p);

    void refreshLayersList();
    void refreshProperties();
    void refreshArt();
    void refreshSurfaceTypes();
    void refreshShapes();
    void setUpScrolling();
    void setControlEnableStates();

    static void initializePixmapCache();

    EditorObject::ObjectGraphic * graphicAt(int x, int y);

    bool overGraphicLeft(int x, int y);
    bool overGraphicTop(int x, int y);
    bool overGraphicRight(int x, int y);
    bool overGraphicBottom(int x, int y);
    bool overSelectedGraphic(int x, int y);

    void determineCursor();

    void doDragAction(int x, int y);

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
