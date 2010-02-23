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

    out->m_name = QFileInfo(file).fileName();

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

void EditorWorld::save()
{
    // save each map
    for (unsigned int i=0; i<m_maps.size(); ++i) {
        EditorMap * map = (EditorMap *) m_maps.at(i);
        map->save();
    }

    // save the world
    QString filename = QDir(EditorResourceManager::worldsDir()).absoluteFilePath(m_name);
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    assert(file.isOpen());
    if( ! file.isOpen() ){
        qDebug() << "Unable to open " << filename << " for writing";
        return;
    }

    QTextStream out(&file);

    out << "version=1\n\n";
    out << "# A world is a list of map declarations and where they are.\n";
    out << "# map=x,y,z,id\n";
    for (unsigned int i=0; i<m_maps.size(); ++i) {
        EditorMap * map = (EditorMap *) m_maps.at(i);
        // TODO: support stories
        out << "map=" << map->left() << "," << map->top() << "," << 0 << "," << map->name() << "\n";
    }

}
