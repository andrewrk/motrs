#include "WorldEditor.h"
#include "ui_WorldEditor.h"

#include "SettingsDialog.h"
#include "EditorResourceManager.h"
#include "WorldView.h"
#include "ObjectEditor.h"
#include "EditorGraphic.h"
#include "EditorSettings.h"
#include "EditorUniverse.h"

#include "Config.h"
#include "MainWindow.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QListWidget>
#include <QDebug>
#include <QMessageBox>
#include <QInputDialog>

#include "moc_WorldEditor.cxx"

WorldEditor::WorldEditor(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::WorldEditor),
    m_view(new WorldView(this, this)),
    m_toolNames(),
    m_playtestProcess(NULL)
{
    m_ui->setupUi(this);

    // set up menus
    m_ui->actionQuit->setShortcut(QKeySequence(Qt::AltModifier | Qt::Key_F4));

    m_objectMenu = new QMenu(m_ui->lstObjects);
    m_objectMenu->addAction(tr("&Edit Object"), this, SLOT(editSelectedObject()));
    m_objectMenu->addAction(tr("&Delete Object"), this, SLOT(deleteSelectedObject()));
    m_objectMenu->addSeparator();
    m_objectMenu->addAction(tr("Create &New Object"), this, SLOT(on_btnNewObject_clicked()));

    m_ui->lstObjects->setContextMenuPolicy(Qt::CustomContextMenu);

    m_worldMenu = new QMenu(m_ui->lstWorlds);
    m_worldMenu->addAction(tr("&Edit World"), this, SLOT(editSelectedWorld()));
    m_worldMenu->addAction(tr("&Rename World"), this, SLOT(renameSelectedWorld()));
    m_worldMenu->addAction(tr("&Delete World"), this, SLOT(deleteSelectedWorld()));
    m_worldMenu->addSeparator();
    m_worldMenu->addAction(tr("Create &New World"), this, SLOT(on_btnNewWorld_clicked()));

    m_ui->lstWorlds->setContextMenuPolicy(Qt::CustomContextMenu);


    EditorGraphic::initialize();

    // fill tools combo box with appropriate values
    fillToolComboBox(*m_ui->cboLeftClick);
    fillToolComboBox(*m_ui->cboMiddleClick);
    fillToolComboBox(*m_ui->cboRightClick);


    // hook window menu clicks to dock show/hide
    connect(m_ui->actionEntities, SIGNAL(triggered(bool)), m_ui->dockEntities, SLOT(setShown(bool)));
    connect(m_ui->actionLayers, SIGNAL(triggered(bool)), m_ui->dockLayers, SLOT(setShown(bool)));
    connect(m_ui->actionObjects, SIGNAL(triggered(bool)), m_ui->dockObjects, SLOT(setShown(bool)));
    connect(m_ui->actionTools, SIGNAL(triggered(bool)), m_ui->dockTools, SLOT(setShown(bool)));
    connect(m_ui->actionTriggers, SIGNAL(triggered(bool)), m_ui->dockTriggers, SLOT(setShown(bool)));
    connect(m_ui->actionWorlds, SIGNAL(triggered(bool)), m_ui->dockWorlds, SLOT(setShown(bool)));

    connect(m_ui->dockEntities, SIGNAL(visibilityChanged(bool)), m_ui->actionEntities, SLOT(setChecked(bool)));
    connect(m_ui->dockLayers, SIGNAL(visibilityChanged(bool)), m_ui->actionLayers, SLOT(setChecked(bool)));
    connect(m_ui->dockObjects, SIGNAL(visibilityChanged(bool)), m_ui->actionObjects, SLOT(setChecked(bool)));
    connect(m_ui->dockTools, SIGNAL(visibilityChanged(bool)), m_ui->actionTools, SLOT(setChecked(bool)));
    connect(m_ui->dockTriggers, SIGNAL(visibilityChanged(bool)), m_ui->actionTriggers, SLOT(setChecked(bool)));
    connect(m_ui->dockWorlds, SIGNAL(visibilityChanged(bool)), m_ui->actionWorlds, SLOT(setChecked(bool)));

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


void WorldEditor::editSelectedObject()
{
    QListWidgetItem * item = m_ui->lstObjects->currentItem();

    if (item == NULL)
        return;

    ObjectEditor * editor = new ObjectEditor(this);
    m_objectWindows.append(editor);
    editor->open(item->data(Qt::UserRole).toString());
    editor->show();
}

void WorldEditor::deleteSelectedObject()
{
    QListWidgetItem * item = m_ui->lstObjects->currentItem();

    if (item == NULL)
        return;

    QString file = item->data(Qt::UserRole).toString();
    QFile::remove(file);
    refreshObjectList();
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
    // make sure we're not going to clobber someone's work
    if (! m_view->guiEnsureSaved()) {
        e->ignore();
        return;
    }

    // close all other forms
    for (int i=0; i<m_objectWindows.size(); ++i) {
        ObjectEditor * editor = m_objectWindows.at(i);
        editor->close();

        if (editor->isVisible()) {
            e->ignore();
            return;
        }
    }

    EditorSettings::setWorldEditorState(this->saveState());
    EditorSettings::setWorldEditorGeometry(this->saveGeometry());

    EditorSettings::setWorldEditorToolLeft(m_view->toolLeftClick());
    EditorSettings::setWorldEditorToolMiddle(m_view->toolMiddleClick());
    EditorSettings::setWorldEditorToolRight(m_view->toolRightClick());



    e->accept();
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
        m_toolNames << "Create Map";
        m_toolNames << "Set Real Starting Point";
        m_toolNames << "Set Test Starting Point";
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
    m_view->refreshGui();
}

void WorldEditor::refreshWorldList()
{
    m_ui->lstWorlds->clear();

    // do a directory listing of data/worlds
    QDir dir(EditorResourceManager::worldsDir());
    QStringList filters;
    filters << "*.world";
    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    m_ui->lstWorlds->addItems(entries);
}

void WorldEditor::refreshObjectList()
{
    m_view->refreshObjectsList();
}

void WorldEditor::on_lstWorlds_doubleClicked(QModelIndex index)
{
    editSelectedWorld();
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
    return m_ui->lstLayers;
}

QListWidget * WorldEditor::objectsList()
{
    return m_ui->lstObjects;
}

QPushButton * WorldEditor::newLayerButton()
{
    return m_ui->btnNewLayer;
}

QPushButton * WorldEditor::newObjectButton()
{
    return m_ui->btnNewObject;
}

QPushButton * WorldEditor::newWorldButton()
{
    return m_ui->btnNewWorld;
}

QPushButton * WorldEditor::newEntityButton()
{
    return m_ui->btnNewEntity;
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

void WorldEditor::on_lstLayers_itemSelectionChanged()
{
    m_view->setSelectedLayer(m_ui->lstLayers->currentRow());
}

void WorldEditor::on_lstLayers_itemChanged(QListWidgetItem* item)
{
    m_view->update();
}

void WorldEditor::on_btnMoveLayerUp_clicked()
{
    if( m_ui->lstLayers->currentRow() > 0 )
        m_view->swapLayers(m_ui->lstLayers->currentRow(), m_ui->lstLayers->currentRow()-1);
}

void WorldEditor::on_btnMoveLayerDown_clicked()
{
    if( m_ui->lstLayers->currentRow() < m_ui->lstLayers->count() - 1 )
        m_view->swapLayers(m_ui->lstLayers->currentRow(), m_ui->lstLayers->currentRow()+1);
}

void WorldEditor::on_btnNewLayer_clicked()
{
    m_view->addLayer();
}

void WorldEditor::on_btnDeleteLayer_clicked()
{
    if( m_ui->lstLayers->currentRow() > -1 )
        m_view->deleteLayer(m_ui->lstLayers->currentRow());
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
    editSelectedObject();
}

void WorldEditor::on_actionSave_triggered()
{
    m_view->guiSave();
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

void WorldEditor::on_lstObjects_customContextMenuRequested(QPoint pos)
{
    m_objectMenu->popup(m_ui->lstObjects->mapToGlobal(pos));
}

void WorldEditor::on_btnNewWorld_clicked()
{
    // create an empty world and save it to disk
    EditorWorld * world = EditorWorld::createEmpty(tr("New World.world"));
    world->save();
    delete world;

    refreshWorldList();
}


void WorldEditor::editSelectedWorld()
{
    QListWidgetItem * item = m_ui->lstWorlds->currentItem();
    if (item == NULL)
        return;

    QDir dir(EditorResourceManager::worldsDir());
    openWorld(dir.absoluteFilePath(item->text()));
}

void WorldEditor::renameSelectedWorld()
{
    QListWidgetItem * item = m_ui->lstWorlds->currentItem();

    if (item == NULL)
        return;

    QString newName = QInputDialog::getText(this, tr("Rename World"),
        tr("New name for the world:"), QLineEdit::Normal, item->text());

    if (! newName.isNull()) {
        QDir worldsDir = QDir(EditorResourceManager::worldsDir());
        QString oldFile = worldsDir.absoluteFilePath(item->text());
        QString newFile = worldsDir.absoluteFilePath(newName);
        if (QFileInfo(newFile).exists()) {
            if (QMessageBox::question(this, tr("File exists"),
                tr("A world with that name already exists. Do you want to overwrite it?"),
                QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
            {
                return;
            }
            QFile::remove(newFile);
        }

        // replace on disk
        if (! QFile::copy(oldFile, newFile))
            return;
        QFile::remove(oldFile);

        // replace in universe
        EditorUniverse * universe = EditorUniverse::load(EditorResourceManager::universeFile());
        assert(universe);
        universe->renameWorld(item->text(), newName);
        universe->save();
        delete universe;

        refreshWorldList();
    }
}

void WorldEditor::deleteSelectedWorld()
{
    QListWidgetItem * item = m_ui->lstWorlds->currentItem();

    if (item == NULL)
        return;

    // this is dangerous. confirm
    if (QMessageBox::question(this, tr("Confirm delete world"),
        tr("Are you sure you want to delete this world?"),
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No) == QMessageBox::No)
    {
        return;
    }

    // delete from universe
    EditorUniverse * universe = EditorUniverse::load(EditorResourceManager::universeFile());
    assert(universe);
    universe->removeWorld(item->text());
    universe->save();
    delete universe;

    // delete from disk
    QFile::remove(QDir(EditorResourceManager::worldsDir()).absoluteFilePath(item->text()));

    refreshWorldList();
}


void WorldEditor::on_lstWorlds_customContextMenuRequested(QPoint pos)
{
    m_worldMenu->popup(m_ui->lstWorlds->mapToGlobal(pos));
}

void WorldEditor::on_actionTest_triggered()
{
    m_view->guiSave();

    if (m_playtestProcess != NULL) {
        m_playtestProcess->close();
        deletePlaytestProcess();
    }

    QString exeFile = QDir(QApplication::applicationDirPath()).absoluteFilePath("motrs.exe");
    if (! QFileInfo(exeFile).exists()) {
        exeFile = QDir(QApplication::applicationDirPath()).absoluteFilePath("motrs");
        if (! QFileInfo(exeFile).exists()) {
            QMessageBox::critical(this, tr("Error playtesting"), tr("Error playtesting: Could not find motrs game executable."));
            return;
        }
    }

    // call build on the universe and generate a resources.dat file
    // in this folder ready to be played.
    ResourceFile resources(QDir(QApplication::applicationDirPath()).absoluteFilePath("resources.dat").toStdString());
    if (! resources.isOpen()) {
        QMessageBox::critical(this, tr("Error playtesting"), tr("Error playtesting: could not open the resources file"));
        return;
    }

    EditorUniverse * universe = EditorUniverse::load(EditorResourceManager::testUniverseFile());
    assert(universe);
    bool ok = universe->build(resources);

    delete universe;

    if (! ok) {
        QMessageBox::critical(this, tr("Error playtesting"), tr("Error playtesting: Unable to build the universe"));
        return;
    }

    // we're debugging, so run the game in the same thread so that if it crashes we get to debug, yay!
    Config::initialize(QDir(QApplication::applicationDirPath()).absoluteFilePath("config.ini").toStdString());
    MainWindow().exec();
    return;

    // start the gameplay thread
    m_playtestProcess = new QProcess(this);
    m_playtestProcess->connect(m_playtestProcess, SIGNAL(finished(int)), this, SLOT(deletePlaytestProcess(int)));
    m_playtestProcess->startDetached(exeFile);
}

void WorldEditor::deletePlaytestProcess(int returnCode)
{
    delete m_playtestProcess;
    m_playtestProcess = NULL;

    if (returnCode != 0)
        QMessageBox::critical(this, tr("Error playtesting"), tr("Error playtesting: Game crashed."));
}

void WorldEditor::keyPressEvent(QKeyEvent * e)
{
    if (e->key() == Qt::Key_F2) {
        // rename
        if (m_ui->lstWorlds->hasFocus()) {
            renameSelectedWorld();
            e->accept();
        }
    }

    e->ignore();
}
