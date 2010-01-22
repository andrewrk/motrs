#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "SettingsDialog.h"
#include "EditorUtils.h"

#include <QDir>
#include <QGraphicsPixmapItem>
#include <QListWidget>
#include <QDebug>

#include "moc_MainWindow.cxx"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_view(new WorldView(this, this)),
    m_scene(new QGraphicsScene()),
    m_mouseState(Normal)
{
    ui->setupUi(this);

    setCentralWidget(m_view);

    ui->dock_art->setWidget(ui->widget_art);
    ui->view_art->setScene(m_scene);
}

MainWindow::~MainWindow()
{
    delete ui;
    cleanupArt();
    delete m_view;
    delete m_scene;

}

void MainWindow::resizeEvent(QResizeEvent * e)
{

}

void MainWindow::showEvent(QShowEvent * e)
{
    // load universe

    refreshArt();
    refreshWorldList();
}

void MainWindow::on_actionSettings_triggered()
{
    SettingsDialog::instance()->exec();
}

void MainWindow::refreshWorldList()
{
    ui->list_worlds->clear();

    // do a directory listing of data/worlds
    QDir dir(EditorUtils::dataDir());
    dir.cd("worlds");
    QStringList filters;
    filters << "*.world";
    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    ui->list_worlds->addItems(entries);
}

void MainWindow::refreshArt()
{
    m_scene->clear();
    cleanupArt();

    // do a directory listing of data/art
    QDir dir(EditorUtils::dataDir());
    dir.cd("art");
    QStringList filters;
    filters << "*.bmp" << "*.png" << "*.jpg";
    QStringList entries = dir.entryList(filters, QDir::Files | QDir::Readable,
        QDir::Name | QDir::IgnoreCase);
    int paneWidth = 0, paneHeight = 0;
    for(int i=0; i<entries.size(); ++i) {
        // create item
        QPixmap * pixmap = new QPixmap(dir.absoluteFilePath(entries[i]));
        QGraphicsPixmapItem * item = m_scene->addPixmap(*pixmap);

        // position the item
        QSize size = pixmap->size();
        item->setPos(0, paneHeight);
        paneWidth = size.width() > paneWidth ? size.width() : size.height();
        paneHeight += size.height();

        // keep track for memory purposes
        ArtItem artItem;
        artItem.graphicsPixmapItem = item;
        artItem.pixmap = pixmap;

        m_art.insert(item, artItem);
    }

    ui->view_art->setSceneRect(0, 0, paneWidth, paneHeight);
}

void MainWindow::cleanupArt()
{
    for(QMap<QGraphicsPixmapItem *, ArtItem>::const_iterator i =
        m_art.constBegin(); i != m_art.constEnd(); ++i)
    {
        delete i.value().pixmap;
    }

    m_art.clear();
}

void MainWindow::on_actionSetStartingPoint_triggered()
{
    m_mouseState = SetStartingPoint;
}


void MainWindow::on_list_worlds_doubleClicked(QModelIndex index)
{
    QListWidgetItem * item = ui->list_worlds->item(index.row());
    QDir dir(EditorUtils::dataDir());
    dir.cd("worlds");
    openWorld(dir.absoluteFilePath(item->text()));
}

void MainWindow::openWorld(QString file)
{
    m_view->setWorld(new EditorWorld(file));
}
