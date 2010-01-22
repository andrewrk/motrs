#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include <QSettings>


#include "moc_SettingsDialog.cxx"


SettingsDialog * SettingsDialog::s_inst = NULL;

SettingsDialog::SettingsDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::SettingsDialog)
{
    m_ui->setupUi(this);
}

SettingsDialog::~SettingsDialog()
{
    delete m_ui;
}

void SettingsDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void SettingsDialog::showEvent(QShowEvent * e)
{
    QSettings settings;
    m_ui->txtDataFolder->setPlainText(settings.value("paths/data", ".").toString());
}

SettingsDialog * SettingsDialog::instance()
{
    if( ! s_inst )
        s_inst = new SettingsDialog();
    return s_inst;
}






void SettingsDialog::on_btnOK_clicked()
{
    QSettings settings;
    settings.setValue("paths/data", m_ui->txtDataFolder->toPlainText());
    accept();
}



void SettingsDialog::on_btnCancel_clicked()
{
    reject();
}
