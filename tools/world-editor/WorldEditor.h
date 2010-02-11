#ifndef WORLDEDITOR_H
#define WORLDEDITOR_H

#include <QtGui/QMainWindow>
#include <QListWidget>
#include <QMap>
#include <QModelIndex>
#include <QComboBox>
#include <QPushButton>

class WorldView;

namespace Ui
{
    class WorldEditor;
}

class WorldEditor : public QMainWindow
{
    Q_OBJECT
    friend class WorldView;
public:
    WorldEditor(QWidget *parent = 0);
    ~WorldEditor();

protected:
    void resizeEvent(QResizeEvent * e);
    void showEvent(QShowEvent * e);
    void closeEvent(QCloseEvent * e);

private:
    enum MouseTool {
        Nothing,
        Arrow,
        Eraser,
        Pan,
        Center,
        Pencil,
        Brush,
    };

    Ui::WorldEditor * m_ui;

    WorldView * m_view;

    MouseTool m_toolLeftClick;
    MouseTool m_toolMiddleClick;
    MouseTool m_toolRightClick;

    QStringList m_toolNames;

    void refreshWorldList();
    void openWorld(QString file);
    void fillToolComboBox(QComboBox & comboBox);


    QListWidget * layersList();
    QPushButton * newLayerButton();
    QPushButton * deleteLayerButton();
    QPushButton * moveLayerUpButton();
    QPushButton * moveLayerDownButton();

private slots:
    void on_actionQuit_triggered();
    void on_btnDeleteLayer_clicked();
    void on_btnNewLayer_clicked();
    void on_btnMoveLayerDown_clicked();
    void on_btnMoveLayerUp_clicked();
    void on_list_layers_itemChanged(QListWidgetItem* item);
    void on_list_layers_itemSelectionChanged();
    void on_list_worlds_doubleClicked(QModelIndex index);

    void on_actionSettings_triggered();
    void on_actionObjectEditor_triggered();

    void on_cboLeftClick_currentIndexChanged(int index);
    void on_cboMiddleClick_currentIndexChanged(int index);
    void on_cboRightClick_currentIndexChanged(int index);


};

#endif // WORLDEDITOR_H
