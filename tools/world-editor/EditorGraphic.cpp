#include "EditorGraphic.h"

#include "EditorResourceManager.h"

#include <QFileInfo>
#include <QColor>
#include <QObject>
#include <QDir>
#include <QDebug>
#include <QBitmap>

long long int EditorGraphic::s_frameCount = 0;
const int EditorGraphic::c_gameFps = 60;
EditorGraphicTimer * EditorGraphic::s_timer = NULL;

EditorGraphic::EditorGraphic() :
    m_frames(),
    m_frameCount(1),
    m_fps(1),
    m_offset(0)
{
}

EditorGraphic * EditorGraphic::load(QString filename)
{
    EditorGraphic * out = new EditorGraphic();

    // check if it exists as a static image
    QFileInfo info(filename);

    if (info.isFile()) {
        // static image
        QPixmap * pixmap = new QPixmap(filename);
        pixmap->setMask(pixmap->createMaskFromColor(QColor(255, 0, 255), Qt::MaskInColor));
        out->m_frames.append(pixmap);
        out->m_fps = 1;
        out->m_frameCount = 1;
        out->m_width = pixmap->width();
        out->m_height = pixmap->height();
    } else if (info.isDir()) {
        // it's an animation
        QDir dir(filename);

        // read the properties.txt file
        QList< QPair<QString, QString> > props;
        if (! EditorResourceManager::loadTextFile(dir.absoluteFilePath("properties.txt"), props)) {
            qDebug() << "Unable to read properties file for animation: " << filename;
            delete out;
            return NULL;
        }

        QColor alpha;
        for (int i=0; i<props.size(); ++i) {
            if (props[i].first.compare("transparent") == 0) {
                QStringList color = props[i].second.split(",");
                alpha.setRgb(color.at(0).toInt(), color.at(1).toInt(), color.at(2).toInt());
            } else if (props[i].first.compare("fps") == 0) {
                out->m_fps = props[i].second.toInt();
            } else {
                qDebug() << "Unrecognized property in animation: " << props[i].first;
                delete out;
                return NULL;
            }
        }

        // create the frames
        out->m_width = 0;
        out->m_height = 0;
        QStringList frameFiles = dir.entryList(QDir::NoDotAndDotDot|QDir::Files, QDir::Name);
        frameFiles.removeOne("properties.txt");
        out->m_frameCount = frameFiles.size();
        for (int i=0; i<out->m_frameCount; ++i) {
            QPixmap * pixmap = new QPixmap(dir.absoluteFilePath(frameFiles[i]));
            pixmap->setMask(pixmap->createMaskFromColor(alpha, Qt::MaskInColor));
            out->m_width = pixmap->width() > out->m_width ? pixmap->width() : out->m_width;
            out->m_height = pixmap->height() > out->m_height ? pixmap->height() : out->m_height;
            out->m_frames.append(pixmap);
        }
    }

    return out;
}

void EditorGraphic::updateFrameCount()
{
    ++s_frameCount;
}

void EditorGraphic::initialize()
{
    if( s_timer == NULL )
        s_timer = new EditorGraphicTimer(1000 / c_gameFps);
}

void EditorGraphic::draw(QPainter & p, int x, int y, int width, int height)
{
    p.drawPixmap(x, y, width, height, *m_frames.at(currentFrame()));
}

void EditorGraphic::draw(QPainter & p, int x, int y)
{
    p.drawPixmap(x, y, *m_frames.at(currentFrame()));
}

// calculate which frame to draw
int EditorGraphic::currentFrame() {
    return (s_frameCount * m_fps / c_gameFps + m_offset) % m_frameCount;
}

QPixmap * EditorGraphic::toPixmap(int frame)
{
    if (frame == -1)
        frame = currentFrame();
    return m_frames.at(frame % m_frameCount);
}
