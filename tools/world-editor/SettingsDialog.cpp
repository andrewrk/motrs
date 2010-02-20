#include "SettingsDialog.h"
#include "ui_SettingsDialog.h"

#include "WorldView.h"
#include "EditorSettings.h"

#include <QFileDialog>

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
    m_ui->txtDataFolder->setText(EditorSettings::dataFolder());

    EditorSettings::GridRenderType grid = EditorSettings::gridRenderType();

    switch(grid) {
        case EditorSettings::None:
            m_ui->optGridOff->setChecked(true);
            break;
        case EditorSettings::Fast:
            m_ui->optGridFast->setChecked(true);
            break;
        case EditorSettings::Pretty:
            m_ui->optGridPretty->setChecked(true);
            break;
        case EditorSettings::Solid:
            m_ui->optGridSolid->setChecked(true);
    }
}

SettingsDialog * SettingsDialog::instance()
{
    if( ! s_inst )
        s_inst = new SettingsDialog();
    return s_inst;
}

void SettingsDialog::on_btnOK_clicked()
{
    EditorSettings::setDataFolder(m_ui->txtDataFolder->text());

    EditorSettings::GridRenderType grid;
    if( m_ui->optGridOff->isChecked() )
        grid = EditorSettings::None;
    else if(m_ui->optGridFast->isChecked() )
        grid = EditorSettings::Fast;
    else if(m_ui->optGridPretty->isChecked() )
        grid = EditorSettings::Pretty;
    else if(m_ui->optGridSolid->isChecked() )
        grid = EditorSettings::Solid;

    EditorSettings::setGridRenderType(grid);

    accept();
}

void SettingsDialog::on_btnCancel_clicked()
{
    reject();
}

void SettingsDialog::on_btnBrowseDataFolder_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this,
        tr("Browse for data folder"), m_ui->txtDataFolder->text());

    if( ! dir.isEmpty() )
        m_ui->txtDataFolder->setText(dir);
}
