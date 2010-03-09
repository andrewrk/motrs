#include "WorldEditor.h"
#include "ui_WorldEditor.h"

#include "SettingsDialog.h"
#include "EditorResourceManager.h"
#include "WorldView.h"
#include "ObjectEditor.h"
#include "EditorGraphic.h"
#include "EditorSettings.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QListWidget>
#include <QDebug>
#include <QMessageBox>

#include "moc_WorldEditor.cxx"

WorldEditor::WorldEditor(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::WorldEditor),
    m_view(new WorldView(this, this)),
    m_toolNames()
{
    m_ui->setupUi(this);

    m_ui->actionQuit->setShortcut(QKeySequence(Qt::AltModifier | Qt::Key_F4));

    EditorGraphic::initialize();

    // fill tools combo box with appropriate values
    fillToolComboBox(*m_ui->cboLeftClick);
    fillToolComboBox(*m_ui->cboMiddleClick);
    fillToolComboBox(*m_ui->cboRightClick);

    // restore window size and dock state from settings
    this->restoreState(EditorSettings::worldEditorState());
    this->restoreGeometry(EditorSettings::worldEditorGeometry());

    m_ui->cboLeftClick->setCurrentIndex(EditorSettings::worldEditorToolLeft());
    m_ui->cboMiddleClick->setCurrentIndex(EditorSettings::worldEditorToolMiddle());
    m_ui->cboRightClick->setCurrentIndex(EditorSettings::worldEditorToolRight());

    m_view->setFocusPolicy(Qt::StrongFocus);
    setCentralWidget(m_view);
    m_view->refreshGui();
}

void WorldEditor::on_cboLeftClick_currentIndexChanged(int index)
{
    m_view->setToolLeftClick( (WorldView::MouseTool) index);
}
void WorldEditor::on_cboMiddleClick_currentIndexChanged(int index)
{
    m_view->setToolMiddleClick( (WorldView::MouseTool) index);
}
void WorldEditor::on_cboRightClick_currentIndexChanged(int index)
{
    m_view->setToolRightClick( (WorldView::MouseTool) index);
}

void WorldEditor::closeEvent(QCloseEvent * e)
{
    EditorSettings::setWorldEditorState(this->saveState());
    EditorSettings::setWorldEditorGeometry(this->saveGeometry());

    EditorSettings::setWorldEditorToolLeft(m_view->toolLeftClick());
    EditorSettings::setWorldEditorToolMiddle(m_view->toolMiddleClick());
    EditorSettings::setWorldEditorToolRight(m_view->toolRightClick());

    // TODO: make sure we're not going to clobber someone's work

    // close all other forms
    for (int i=0; i<m_objectWindows.size(); ++i)
        m_objectWindows.at(i)->close();

    this->close();
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
    QDir dir(EditorResourceManager::worldsDir());
    QStringList filters;
    filters << "*.world";
    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    m_ui->list_worlds->addItems(entries);
}

void WorldEditor::refreshObjectList()
{
    m_view->refreshObjectsList();
}

void WorldEditor::on_list_worlds_doubleClicked(QModelIndex index)
{
    QListWidgetItem * item = m_ui->list_worlds->item(index.row());
    QDir dir(EditorResourceManager::worldsDir());
    openWorld(dir.absoluteFilePath(item->text()));
}

void WorldEditor::openWorld(QString file)
{
    EditorWorld * world = EditorWorld::load(file);

    if (world == NULL) {
        QMessageBox::warning(this, tr("Error"), tr("Error opening world."));
        return;
    }
    m_view->setWorld(world);
}

QListWidget * WorldEditor::layersList()
{
    return m_ui->list_layers;
}

QListWidget * WorldEditor::objectsList()
{
    return m_ui->lstObjects;
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
    m_objectWindows.append(editor);
    connect(editor, SIGNAL(objectSaved()), this, SLOT(refreshObjectList()));
    editor->createEmpty();
    editor->show();
}

void WorldEditor::on_btnNewEntity_clicked()
{
    // TODO: create a new entity and open it up in the editor
}

void WorldEditor::on_lstObjects_itemDoubleClicked(QListWidgetItem* item)
{
    ObjectEditor * editor = new ObjectEditor(this);
    m_objectWindows.append(editor);
    editor->open(item->data(Qt::UserRole).toString());
    editor->show();
}

void WorldEditor::on_actionSave_triggered()
{
    m_view->saveTheWorld();
}

void WorldEditor::on_actionDelete_triggered()
{
    m_view->deleteSelection();
}

void WorldEditor::on_actionCut_triggered()
{
    m_view->copySelection();
    m_view->deleteSelection();
}

void WorldEditor::on_actionCopy_triggered()
{
    m_view->copySelection();
}

void WorldEditor::on_actionPaste_triggered()
{
    m_view->paste();
}

void WorldEditor::on_actionSelectAll_triggered()
{
    m_view->selectAll();
}
