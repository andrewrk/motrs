#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QGraphicsScene>
#include <QMap>
#include <QModelIndex>

#include "WorldView.h"

namespace Ui
{
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void resizeEvent(QResizeEvent * e);
    void showEvent(QShowEvent * e);

private:
    typedef struct {
        QGraphicsPixmapItem * graphicsPixmapItem;
        QPixmap * pixmap;
    } ArtItem;

    enum MouseState {
        Normal,
        SetStartingPoint,
    };

    Ui::MainWindow *ui;

    WorldView * m_view;
    QGraphicsScene * m_scene;
    QMap<QGraphicsPixmapItem *, ArtItem> m_art;

    MouseState m_mouseState;

    void refreshArt();
    void refreshWorldList();
    void cleanupArt();
    void openWorld(QString file);

private slots:
    void on_list_worlds_doubleClicked(QModelIndex index);
    void on_actionSetStartingPoint_triggered();
    void on_actionSettings_triggered();

    friend class WorldView;
};

#endif // MAINWINDOW_H
