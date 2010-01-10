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
    void on_SettingsDialog_rejected();
    void on_SettingsDialog_accepted();
};

#endif // SETTINGSDIALOG_H
