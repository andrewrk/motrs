#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SettingsDialog.h"
#include "EditorResourceManager.h"
#include "WorldView.h"
#include "ObjectEditor.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QListWidget>
#include <QDebug>
#include <QSettings>

#include "moc_MainWindow.cxx"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_view(new WorldView(this, this)),
    m_toolNames()
{
    ui->setupUi(this);

    m_view->setFocusPolicy(Qt::StrongFocus);

    setCentralWidget(m_view);

    // fill tools combo box with appropriate values
    fillToolComboBox(*ui->cboLeftClick);
    fillToolComboBox(*ui->cboMiddleClick);
    fillToolComboBox(*ui->cboRightClick);

    // load window/dock state
    QFile defaultLayoutFile("default-layout");
    QByteArray defaultLayout;
    bool haveDefaultLayout = false;
    if( defaultLayoutFile.open(QIODevice::ReadOnly) ) {
        defaultLayout = defaultLayoutFile.readAll();
        haveDefaultLayout = true;
    }

    QSettings settings;
    this->restoreState(settings.value("windowState",
        haveDefaultLayout ? defaultLayout : QVariant()).toByteArray());
    this->restoreGeometry(settings.value("windowGeometry").toByteArray());

    ui->cboLeftClick->setCurrentIndex(settings.value("state/tool/left", Arrow).toInt());
    ui->cboMiddleClick->setCurrentIndex(settings.value("state/tool/middle", Pan).toInt());
    ui->cboRightClick->setCurrentIndex(settings.value("state/tool/right", Eraser).toInt());
}

void MainWindow::on_cboLeftClick_currentIndexChanged(int index)
{
    m_toolLeftClick = (MouseTool) index;
}
void MainWindow::on_cboMiddleClick_currentIndexChanged(int index)
{
    m_toolMiddleClick = (MouseTool) index;
}
void MainWindow::on_cboRightClick_currentIndexChanged(int index)
{
    m_toolRightClick = (MouseTool) index;
}

void MainWindow::closeEvent(QCloseEvent * e)
{
    QSettings settings;
    settings.setValue("windowState", this->saveState());
    settings.setValue("windowGeometry", this->saveGeometry());

    settings.setValue("state/tool/left", m_toolLeftClick);
    settings.setValue("state/tool/middle", m_toolMiddleClick);
    settings.setValue("state/tool/right", m_toolRightClick);
}

void MainWindow::fillToolComboBox(QComboBox & cbo)
{
    if( m_toolNames.size() == 0 ) {
        m_toolNames << "Nothing";
        m_toolNames << "Arrow";
        m_toolNames << "Erasor";
        m_toolNames << "Pan";
        m_toolNames << "Center";
        m_toolNames << "Pencil";
        m_toolNames << "Brush";
        m_toolNames << "SetStartingPoint";
    }

    cbo.clear();
    cbo.addItems(m_toolNames);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_view;

}

void MainWindow::resizeEvent(QResizeEvent * e)
{

}

void MainWindow::showEvent(QShowEvent * e)
{
    // load universe
    refreshWorldList();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog::instance()->exec();
    m_view->readSettings();
}

void MainWindow::on_actionObjectEditor_triggered()
{
    ObjectEditor::instance()->show();
}

void MainWindow::refreshWorldList()
{
    ui->list_worlds->clear();

    // do a directory listing of data/worlds
    QDir dir(EditorResourceManager::dataDir());
    dir.cd("worlds");
    QStringList filters;
    filters << "*.world";
    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    ui->list_worlds->addItems(entries);
}

void MainWindow::on_list_worlds_doubleClicked(QModelIndex index)
{
    QListWidgetItem * item = ui->list_worlds->item(index.row());
    QDir dir(EditorResourceManager::dataDir());
    dir.cd("worlds");
    openWorld(dir.absoluteFilePath(item->text()));
}

void MainWindow::openWorld(QString file)
{
    m_view->setWorld(new EditorWorld(file));
}

QListWidget * MainWindow::layersList()
{
    return ui->list_layers;
}

QPushButton * MainWindow::newLayerButton()
{
    return ui->btnNewLayer;
}

QPushButton * MainWindow::deleteLayerButton()
{
    return ui->btnDeleteLayer;
}

QPushButton * MainWindow::moveLayerUpButton()
{
    return ui->btnMoveLayerUp;
}

QPushButton * MainWindow::moveLayerDownButton()
{
    return ui->btnMoveLayerDown;
}



void MainWindow::on_list_layers_itemSelectionChanged()
{
    m_view->setSelectedLayer(ui->list_layers->currentRow());
    m_view->setControlEnableStates();
}

void MainWindow::on_list_layers_itemChanged(QListWidgetItem* item)
{
    m_view->update();
}

void MainWindow::on_btnMoveLayerUp_clicked()
{
    if( ui->list_layers->currentRow() > 0 )
        m_view->swapLayers(ui->list_layers->currentRow(), ui->list_layers->currentRow()-1);
}

void MainWindow::on_btnMoveLayerDown_clicked()
{
    if( ui->list_layers->currentRow() < ui->list_layers->count() - 1 )
        m_view->swapLayers(ui->list_layers->currentRow(), ui->list_layers->currentRow()+1);
}

void MainWindow::on_btnNewLayer_clicked()
{
    m_view->addLayer();
}

void MainWindow::on_btnDeleteLayer_clicked()
{
    if( ui->list_layers->currentRow() > -1 )
        m_view->deleteLayer(ui->list_layers->currentRow());
}
