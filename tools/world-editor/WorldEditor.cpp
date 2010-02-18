#include "WorldEditor.h"
#include "ui_WorldEditor.h"
#include "SettingsDialog.h"
#include "EditorResourceManager.h"
#include "WorldView.h"
#include "ObjectEditor.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QListWidget>
#include <QDebug>
#include <QSettings>

#include "moc_WorldEditor.cxx"

WorldEditor::WorldEditor(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::WorldEditor),
    m_view(new WorldView(this, this)),
    m_toolNames()
{
    m_ui->setupUi(this);

    m_ui->actionQuit->setShortcut(QKeySequence(Qt::AltModifier | Qt::Key_F4));

    m_view->setFocusPolicy(Qt::StrongFocus);

    setCentralWidget(m_view);

    // fill tools combo box with appropriate values
    fillToolComboBox(*m_ui->cboLeftClick);
    fillToolComboBox(*m_ui->cboMiddleClick);
    fillToolComboBox(*m_ui->cboRightClick);

    // load window/dock state
    QDir localDataDir(EditorResourceManager::localDataDir());
    QFile defaultLayoutFile(localDataDir.absoluteFilePath("default-layout"));
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

    m_ui->cboLeftClick->setCurrentIndex(settings.value("state/tool/left", Arrow).toInt());
    m_ui->cboMiddleClick->setCurrentIndex(settings.value("state/tool/middle", Pan).toInt());
    m_ui->cboRightClick->setCurrentIndex(settings.value("state/tool/right", Eraser).toInt());
}

void WorldEditor::on_cboLeftClick_currentIndexChanged(int index)
{
    m_toolLeftClick = (MouseTool) index;
}
void WorldEditor::on_cboMiddleClick_currentIndexChanged(int index)
{
    m_toolMiddleClick = (MouseTool) index;
}
void WorldEditor::on_cboRightClick_currentIndexChanged(int index)
{
    m_toolRightClick = (MouseTool) index;
}

void WorldEditor::closeEvent(QCloseEvent * e)
{
    QSettings settings;
    settings.setValue("windowState", this->saveState());
    settings.setValue("windowGeometry", this->saveGeometry());

    settings.setValue("state/tool/left", m_toolLeftClick);
    settings.setValue("state/tool/middle", m_toolMiddleClick);
    settings.setValue("state/tool/right", m_toolRightClick);

    // TODO: make sure we're not going to clobber someone's work

    // close all other forms
    QApplication::quit();

}

void WorldEditor::fillToolComboBox(QComboBox & cbo)
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

WorldEditor::~WorldEditor()
{
    delete m_ui;
    delete m_view;

}

void WorldEditor::resizeEvent(QResizeEvent * e)
{

}

void WorldEditor::showEvent(QShowEvent * e)
{
    // load universe
    refreshWorldList();
}

void WorldEditor::on_actionSettings_triggered()
{
    SettingsDialog::instance()->exec();
    refreshWorldList();
}

void WorldEditor::refreshWorldList()
{
    m_ui->list_worlds->clear();

    // do a directory listing of data/worlds
    QDir dir(EditorResourceManager::dataDir());
    dir.cd("worlds");
    QStringList filters;
    filters << "*.world";
    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    m_ui->list_worlds->addItems(entries);
}

void WorldEditor::on_list_worlds_doubleClicked(QModelIndex index)
{
    QListWidgetItem * item = m_ui->list_worlds->item(index.row());
    QDir dir(EditorResourceManager::dataDir());
    dir.cd("worlds");
    openWorld(dir.absoluteFilePath(item->text()));
}

void WorldEditor::openWorld(QString file)
{
    m_view->setWorld(new EditorWorld(file));
}

QListWidget * WorldEditor::layersList()
{
    return m_ui->list_layers;
}

QPushButton * WorldEditor::newLayerButton()
{
    return m_ui->btnNewLayer;
}

QPushButton * WorldEditor::deleteLayerButton()
{
    return m_ui->btnDeleteLayer;
}

QPushButton * WorldEditor::moveLayerUpButton()
{
    return m_ui->btnMoveLayerUp;
}

QPushButton * WorldEditor::moveLayerDownButton()
{
    return m_ui->btnMoveLayerDown;
}



void WorldEditor::on_list_layers_itemSelectionChanged()
{
    m_view->setSelectedLayer(m_ui->list_layers->currentRow());
    m_view->setControlEnableStates();
}

void WorldEditor::on_list_layers_itemChanged(QListWidgetItem* item)
{
    m_view->update();
}

void WorldEditor::on_btnMoveLayerUp_clicked()
{
    if( m_ui->list_layers->currentRow() > 0 )
        m_view->swapLayers(m_ui->list_layers->currentRow(), m_ui->list_layers->currentRow()-1);
}

void WorldEditor::on_btnMoveLayerDown_clicked()
{
    if( m_ui->list_layers->currentRow() < m_ui->list_layers->count() - 1 )
        m_view->swapLayers(m_ui->list_layers->currentRow(), m_ui->list_layers->currentRow()+1);
}

void WorldEditor::on_btnNewLayer_clicked()
{
    m_view->addLayer();
}

void WorldEditor::on_btnDeleteLayer_clicked()
{
    if( m_ui->list_layers->currentRow() > -1 )
        m_view->deleteLayer(m_ui->list_layers->currentRow());
}

void WorldEditor::on_actionQuit_triggered()
{
    QApplication::quit();
}

void WorldEditor::on_btnNewObject_clicked()
{
    ObjectEditor * editor = new ObjectEditor(this);
    editor->createEmpty();
    editor->show();
}

void WorldEditor::on_btnNewEntity_clicked()
{
    // TODO: create a new entity and open it up in the editor
}
