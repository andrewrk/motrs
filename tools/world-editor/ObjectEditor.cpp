#include "ObjectEditor.h"
#include "ui_ObjectEditor.h"

#include "EditorResourceManager.h"
#include "ObjectView.h"

#include <QDir>
#include <QSettings>

#include "moc_ObjectEditor.cxx"

ObjectEditor::ObjectEditor(QWidget *parent) :
    QMainWindow(parent),
    m_ui(new Ui::ObjectEditor),
    m_view(new ObjectView(this, this))
{
    m_ui->setupUi(this);
    m_ui->actionClose->setShortcut(QKeySequence(Qt::AltModifier | Qt::Key_F4));

    this->setCentralWidget(m_view);

    QSettings settings;
    this->restoreState(settings.value("ObjectEditor/windowState").toByteArray());
    this->restoreGeometry(settings.value("ObjectEditor/windowGeometry").toByteArray());

}

ObjectEditor::~ObjectEditor()
{
    delete m_ui;
}

void ObjectEditor::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ObjectEditor::refreshArt()
{
    // do a directory listing of data/art
    QDir dir(EditorResourceManager::dataDir());
    dir.cd("art");
    QStringList filters;
    filters << "*.bmp" << "*.png" << "*.jpg";
    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    for(int i=0; i<entries.size(); ++i) {
        // create item
        QString file = dir.absoluteFilePath(entries[i]);
        QPixmap * pixmap = new QPixmap(file);
        QListWidgetItem * item = new QListWidgetItem(QIcon(*pixmap), entries[i], m_ui->lstArt);
        item->setData(Qt::UserRole, QVariant(file));
        m_ui->lstArt->addItem(item);
    }
}

QListWidget * ObjectEditor::artList()
{
    return m_ui->lstArt;
}

QListWidget * ObjectEditor::layersList()
{
    return m_ui->lstLayers;
}


QPushButton * ObjectEditor::newLayerButton()
{
    return m_ui->btnNewLayer;
}

QPushButton * ObjectEditor::deleteLayerButton()
{
    return m_ui->btnDeleteLayer;
}

QPushButton * ObjectEditor::moveLayerUpButton()
{
    return m_ui->btnMoveLayerUp;
}

QPushButton * ObjectEditor::moveLayerDownButton()
{
    return m_ui->btnMoveLayerDown;
}

void ObjectEditor::on_lstLayers_currentRowChanged(int currentRow)
{
    m_view->setSelectedLayer(currentRow);
}

void ObjectEditor::open(QString file)
{
    m_view->open(file);
}

void ObjectEditor::on_actionNormal_triggered()
{
    m_ui->actionNormal->setChecked(true);
    m_ui->actionSurfaceType->setChecked(false);
    m_ui->actionShape->setChecked(false);
    m_view->setViewMode(ObjectView::Normal);
}

void ObjectEditor::on_actionSurfaceType_triggered()
{
    m_ui->actionNormal->setChecked(false);
    m_ui->actionSurfaceType->setChecked(true);
    m_ui->actionShape->setChecked(false);
    m_view->setViewMode(ObjectView::SurfaceType);
}

void ObjectEditor::on_actionShape_triggered()
{
    m_ui->actionNormal->setChecked(false);
    m_ui->actionSurfaceType->setChecked(false);
    m_ui->actionShape->setChecked(true);
    m_view->setViewMode(ObjectView::Shape);
}

void ObjectEditor::on_actionZoom100_triggered()
{
    m_view->setZoom(1.0);
}

void ObjectEditor::on_actionZoomIn_triggered()
{
    m_view->zoomIn();
}

void ObjectEditor::on_actionZoomOut_triggered()
{
    m_view->zoomOut();
}

void ObjectEditor::createEmpty()
{
    m_view->createEmpty();
}

void ObjectEditor::on_btnMoveLayerUp_clicked()
{
    if( m_ui->lstLayers->currentRow() > 0 )
        m_view->swapLayers(m_ui->lstLayers->currentRow(), m_ui->lstLayers->currentRow()-1);
}

void ObjectEditor::on_btnMoveLayerDown_clicked()
{
    if( m_ui->lstLayers->currentRow() < m_ui->lstLayers->count() - 1 )
        m_view->swapLayers(m_ui->lstLayers->currentRow(), m_ui->lstLayers->currentRow()+1);
}

void ObjectEditor::on_btnNewLayer_clicked()
{
    m_view->addLayer();
}

void ObjectEditor::on_btnDeleteLayer_clicked()
{
    m_view->deleteLayer(m_ui->lstLayers->currentRow());
}

void ObjectEditor::closeEvent(QCloseEvent * e)
{
    QSettings settings;
    settings.setValue("ObjectEditor/windowState", this->saveState());
    settings.setValue("ObjectEditor/windowGeometry", this->saveGeometry());

}

void ObjectEditor::on_actionClose_triggered()
{
    this->close();
}
