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

    QListWidget * artList();

    static ObjectEditor * instance();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ObjectEditor *m_ui;
    static ObjectEditor * s_inst;

    ObjectView * m_view;

    void refreshArt();
};

#endif // OBJECTEDITOR_H
