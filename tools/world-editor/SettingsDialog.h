#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QtGui/QDialog>

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    ~SettingsDialog();

    static SettingsDialog * instance();

protected:
    void changeEvent(QEvent *e);
    void showEvent(QShowEvent * e);

private:
    SettingsDialog(QWidget *parent = 0);
    Ui::SettingsDialog *m_ui;

    static SettingsDialog * s_inst;

private slots:

private slots:
    void on_btnCancel_clicked();
    void on_btnOK_clicked();
};

#endif // SETTINGSDIALOG_H
