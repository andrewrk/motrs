#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QListWidget>
#include <QGraphicsScene>
#include <QMap>
#include <QModelIndex>
#include <QComboBox>

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
    void closeEvent(QCloseEvent * e);

private:
    enum MouseTool {
        Nothing,
        Arrow,
        Erasor,
        Pan,
        Center,
        Pencil,
        Brush,
        SetStartingPoint,
    };

    typedef struct {
        QGraphicsPixmapItem * graphicsPixmapItem;
        QPixmap * pixmap;
    } ArtItem;

    Ui::MainWindow *ui;

    WorldView * m_view;
    QGraphicsScene * m_scene;
    QMap<QGraphicsPixmapItem *, ArtItem> m_art;

    MouseTool m_toolLeftClick;
    MouseTool m_toolMiddleClick;
    MouseTool m_toolRightClick;

    QStringList m_toolNames;

    void refreshArt();
    void refreshWorldList();
    void cleanupArt();
    void openWorld(QString file);
    QListWidget * layersList();
    void fillToolComboBox(QComboBox & comboBox);

private slots:
    void on_list_worlds_doubleClicked(QModelIndex index);
    void on_actionSettings_triggered();

    friend class WorldView;
};

#endif // MAINWINDOW_H
