#include "EditorWorld.h"

#include "EditorMap.h"
#include "EditorResourceManager.h"

#include <QDebug>
#include <QStringList>
#include <QDir>

EditorWorld::EditorWorld() :
    World()
{
}

EditorWorld::~EditorWorld()
{
}

EditorWorld * EditorWorld::load(QString file)
{
    QList< QPair<QString, QString> > props;

    if(! EditorResourceManager::loadTextFile(file, props) ) {
        qDebug() << "Error loading property file: " << file;
        return NULL;
    }

    EditorWorld * out = new EditorWorld();

    for(int i=0; i<props.size(); ++i) {
        if( props[i].first.compare("version", Qt::CaseInsensitive) == 0 ) {
            int fileVersion = props[i].second.toInt();
            int codeVersion = 1;
            if( fileVersion != codeVersion ) {
                qDebug() << "Tried to open version  " << fileVersion <<
                    " with version " << codeVersion << " code.";
                delete out;
                return NULL;
            }
        } else if( props[i].first.compare("map", Qt::CaseInsensitive) == 0 ) {
            // map=x,y,z,id
            QStringList coords = props[i].second.split(",");
            QDir dir(EditorResourceManager::mapsDir());
            QString mapFile = dir.absoluteFilePath(coords[3]);
            EditorMap * map = EditorMap::load(mapFile);
            assert(map);
            if (map == NULL) {
                qDebug() << "Error loading EditorMap for " << mapFile;
                delete out;
                return NULL;
            }
            map->setPosition(coords[0].toDouble(), coords[1].toDouble(), coords[2].toInt());
            out->m_maps.push_back(map);
        } else {
            qDebug() << "Unrecognized World property: " << props[i].first;
            delete out;
            return NULL;
        }
    }

    out->calculateBoundaries();

    return out;
}
