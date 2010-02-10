#include "ObjectView.h"

#include "EditorResourceManager.h"
#include "ObjectEditor.h"

#include <QPainter>
#include <QStandardItemModel>
#include <QFileInfo>
#include <QDebug>
#include <QMessageBox>

ObjectView::ObjectView() :
    m_dragPixmap(NULL)
{
}

void ObjectView::paintEvent(QPaintEvent * e)
{
    QPainter p(this);
    p.setBackground(Qt::white);
    p.eraseRect(0, 0, this->width(), this->height());

    // draw dragged stuff
    //if( layer == m_selectedLayer && m_dragPixmap != NULL ) {
        p.drawPixmap(m_dragPixmapX, m_dragPixmapY, *m_dragPixmap);
    //}
}

void ObjectView::dropEvent(QDropEvent * e)
{
    QStringList pictureExtensions;
    pictureExtensions << "bmp" << "jpg" << "png";

    QString itemModelMime = "application/x-qabstractitemmodeldatalist";

    // don't show the art in paintevent
    m_dragPixmap = NULL;
    const QMimeData * data = e->mimeData();
    if(data->hasFormat(itemModelMime)) {
        QStandardItemModel model;
        model.dropMimeData(data, Qt::CopyAction, 0, 0, QModelIndex());
        QString file = model.item(0,0)->data(Qt::UserRole).toString();

        QFileInfo info(file);
        QString ext = info.suffix();
        if( pictureExtensions.contains(ext, Qt::CaseInsensitive) ) {
            // it's art
            QString title = info.fileName();
            QPixmap * pixmap = EditorResourceManager::pixmapForArt(title);

        } else if( ext.compare("entity", Qt::CaseInsensitive) ) {
            // it's an entity
            // TODO
        } else if( ext.compare("map", Qt::CaseInsensitive) ) {
            // it's a map
            // TODO
        } else {
            qDebug() << "Unable to drag drop extension " << ext;
            QMessageBox::warning(this, tr("error"), tr("Uh oh, we don't support %1 yet.").arg(ext));
            return;
        }
        e->acceptProposedAction();
    }
}


void ObjectView::dragEnterEvent(QDragEnterEvent * e)
{
    if( e->source() == m_window->artList() ) {
        QStringList pictureExtensions;
        pictureExtensions << "bmp" << "jpg" << "png";

        QString itemModelMime = "application/x-qabstractitemmodeldatalist";

        const QMimeData * data = e->mimeData();
        if(data->hasFormat(itemModelMime)) {
            QStandardItemModel model;
            model.dropMimeData(data, Qt::CopyAction, 0, 0, QModelIndex());
            QString file = model.item(0,0)->data(Qt::UserRole).toString();

            QFileInfo info(file);
            QString ext = info.suffix();
            if( pictureExtensions.contains(ext, Qt::CaseInsensitive) ) {
                // it's art
                QString title = info.fileName();
                qDebug() << title;
                QPixmap * pixmap = EditorResourceManager::pixmapForArt(title);
                m_dragPixmap = pixmap;
            } else if( ext.compare("entity", Qt::CaseInsensitive) ) {
                // it's an entity
                // TODO
            } else if( ext.compare("map", Qt::CaseInsensitive) ) {
                // it's a map
                // TODO
            } else {
                qDebug() << "Unable to drag drop extension " << ext;
                QMessageBox::warning(this, tr("error"), tr("Uh oh, we don't support %1 yet.").arg(ext));
                return;
            }
            e->acceptProposedAction();
        }
    }
}

void ObjectView::dragMoveEvent(QDragMoveEvent * e)
{
    m_dragPixmapX = e->pos().x();
    m_dragPixmapY = e->pos().y();
    this->update();

    e->acceptProposedAction();
}

void ObjectView::dragLeaveEvent(QDragLeaveEvent * e)
{
    m_dragPixmap = NULL;
}
