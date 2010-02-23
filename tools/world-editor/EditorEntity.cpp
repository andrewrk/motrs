#include "EditorEntity.h"

#include "EditorResourceManager.h"
#include "WorldView.h"

#include <QDebug>
#include <QPainter>
#include <QFileInfo>

EditorEntity * EditorEntity::load(QString file)
{
    QList< QPair<QString, QString> > props;

    bool good = EditorResourceManager::loadTextFile(file, props);

    assert(good);
    if (! good) {
        qDebug() << "Error opening Entity file: " << file;
        return NULL;
    }

    EditorEntity * out = new EditorEntity();

    out->m_name = QFileInfo(file).fileName();

    for(int i=0; i<props.size(); ++i) {
        if( props[i].first.compare("version", Qt::CaseInsensitive) == 0 ) {
            int fileVersion = props[i].second.toInt();
            int codeVersion = 6;
            assert(fileVersion == codeVersion);
        } else if( props[i].first.compare("contact", Qt::CaseInsensitive) == 0 ) {
            // contact=cx,cy,r
            QStringList contactProps = props[i].second.split(",");
            out->m_centerOffsetX = contactProps.at(0).toDouble();
            out->m_centerOffsetY = contactProps.at(1).toDouble();
            out->m_radius = contactProps.at(2).toDouble();
        } else if( props[i].first.compare("specs", Qt::CaseInsensitive) == 0 ) {
            // specs = speed, mass
            QStringList specs = props[i].second.split(",");
            out->m_speed = specs.at(0).toDouble();
            out->m_mass = specs.at(1).toDouble();
        } else if( props[i].first.compare("stand", Qt::CaseInsensitive) == 0 ) {
            // nw,w,sw,n,c,s,ne,e,se
            QStringList standGraphics = props[i].second.split(",");
            out->m_graphic = EditorResourceManager::graphic(standGraphics.at(5));
        } else if( props[i].first.compare("walk", Qt::CaseInsensitive) == 0 ) {
            // world editor doesn't care about this
        } else if( props[i].first.compare("run", Qt::CaseInsensitive) == 0 ) {
            // world editor doesn't care about this
        } else if( props[i].first.compare("sword", Qt::CaseInsensitive) == 0 ) {
            // world editor doesn't care about this
        } else {
            // unrecognized entity property
            assert(false);
        }
    }

    return out;
}

EditorEntity::EditorEntity()
{
}

