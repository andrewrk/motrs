#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include <QtGui/QMainWindow>
#include <QListWidget>
#include <QMap>
#include <QModelIndex>
#include <QComboBox>
#include <QPushButton>
#include <QMenu>

class WorldView;
class ObjectEditor;

namespace Ui
{
    class WorldEditor;
}

class WorldEditor : public QMainWindow
{
    Q_OBJECT
public:
    WorldEditor(QWidget *parent = 0);
    ~WorldEditor();

    QListWidget * layersList();
    QListWidget * objectsList();
    QPushButton * newLayerButton();
    QPushButton * deleteLayerButton();
    QPushButton * moveLayerUpButton();
    QPushButton * moveLayerDownButton();

protected:
    void resizeEvent(QResizeEvent * e);
    void showEvent(QShowEvent * e);
    void closeEvent(QCloseEvent * e);

private: //variables
    Ui::WorldEditor * m_ui;

    WorldView * m_view;

    QStringList m_toolNames;

    QMenu * m_objectMenu;
    QMenu * m_worldMenu;

    QList<ObjectEditor *> m_objectWindows;

private: //methods
    void refreshWorldList();
    void openWorld(QString file);
    void fillToolComboBox(QComboBox & comboBox);

private slots:
    void on_lstWorlds_customContextMenuRequested(QPoint pos);
    void on_btnNewWorld_clicked();
    void on_lstObjects_customContextMenuRequested(QPoint pos);
    void on_actionSelectAll_triggered();
    void on_actionPaste_triggered();
    void on_actionCopy_triggered();
    void on_actionCut_triggered();
    void on_actionDelete_triggered();
    void on_actionSave_triggered();
    void on_lstObjects_itemDoubleClicked(QListWidgetItem* item);
    void on_btnNewEntity_clicked();
    void on_btnNewObject_clicked();
    void on_actionQuit_triggered();
    void on_btnDeleteLayer_clicked();
    void on_btnNewLayer_clicked();
    void on_btnMoveLayerDown_clicked();
    void on_btnMoveLayerUp_clicked();
    void on_lstLayers_itemChanged(QListWidgetItem* item);
    void on_lstLayers_itemSelectionChanged();
    void on_lstWorlds_doubleClicked(QModelIndex index);

    void on_actionSettings_triggered();

    void on_cboLeftClick_currentIndexChanged(int index);
    void on_cboMiddleClick_currentIndexChanged(int index);
    void on_cboRightClick_currentIndexChanged(int index);

    void refreshObjectList();

    void editSelectedObject();
    void deleteSelectedObject();

    void editSelectedWorld();
    void renameSelectedWorld();
    void deleteSelectedWorld();

};

#endif // WORLDEDITOR_H
