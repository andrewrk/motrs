#ifndef OBJECTEDITOR_H
#define OBJECTEDITOR_H

#include <QMainWindow>
#include <QListWidget>

class ObjectView;

namespace Ui {
    class ObjectEditor;
}

class ObjectEditor : public QMainWindow {
    Q_OBJECT
public:
    ObjectEditor(QWidget *parent = 0);
    ~ObjectEditor();
    static ObjectEditor * instance();

    void open(QString file);

    QListWidget * artList();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ObjectEditor *m_ui;
    static ObjectEditor * s_inst;

    ObjectView * m_view;

    void refreshArt();

private slots:
    void on_actionShape_triggered();
    void on_actionSurfaceType_triggered();
    void on_actionNormal_triggered();
    void on_listLayers_currentRowChanged(int currentRow);
};

#endif // OBJECTEDITOR_H
