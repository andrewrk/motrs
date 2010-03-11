#include "EditorWorld.h"

#include "EditorMap.h"
#include "EditorResourceManager.h"

#include <QDebug>
#include <QStringList>
#include <QDir>

const int EditorWorld::c_codeVersion = 1;

EditorWorld::EditorWorld() :
    m_left(0),
    m_top(0),
    m_width(0),
    m_height(0)
{
}

EditorWorld::~EditorWorld()
{
}

EditorWorld * EditorWorld::createEmpty(QString name)
{
    EditorWorld * out = new EditorWorld();
    out->m_name = name;
    return out;
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
            if( fileVersion != c_codeVersion ) {
                qDebug() << "Tried to open version  " << fileVersion <<
                    " with version " << c_codeVersion << " code.";
                delete out;
                return NULL;
            }
        } else if( props[i].first.compare("map", Qt::CaseInsensitive) == 0 ) {
            // map=x,y,z,id
            QStringList coords = props[i].second.split(",");
            QDir dir(EditorResourceManager::mapsDir());
            QString mapFile = dir.absoluteFilePath(coords[3]);
            EditorMap * map = EditorMap::load(mapFile);
            map->setWorld(out);
            assert(map);
            if (map == NULL) {
                qDebug() << "Error loading EditorMap for " << mapFile;
                delete map;
                delete out;
                return NULL;
            }
            map->setPosition(coords[0].toInt(), coords[1].toInt(), coords[2].toInt());
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
    for (int i=0; i<m_maps.size(); ++i) {
        EditorMap * map = m_maps.at(i);
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

    out << "version=" << c_codeVersion << "\n\n";
    out << "# A world is a list of map declarations and where they are.\n";
    out << "# map=x,y,z,id\n";
    for (int i=0; i<m_maps.size(); ++i) {
        EditorMap * map = m_maps.at(i);
        // TODO: support stories
        out << "map=" << map->left() << "," << map->top() << "," << 0 << "," << map->name() << "\n";
    }

}

void EditorWorld::addMap(EditorMap * map)
{
    map->setWorld(this);
    m_maps.push_back(map);
    calculateBoundaries();
}

void EditorWorld::calculateBoundaries()
{
    if (m_maps.size() == 0) {
        m_width = 0;
        m_height = 0;
        m_top = 0;
        m_left = 0;
        return;
    }

    EditorMap * map = (EditorMap *) m_maps.at(0);
    int left = map->left();
    int top = map->top();
    int right = left + map->width();
    int bottom = top + map->height();

    for (int i = 1; i < m_maps.size(); i++) {
        map = m_maps.at(i);

        if (left > map->left())
            left = map->left();

        if (top > map->top())
            top = map->top();

        if (right < map->left() + map->width())
            right = map->left() + map->width();

        if (bottom < map->top() + map->height())
            bottom = map->top() + map->height();
    }
    m_left = left;
    m_top = top;
    m_height = bottom - top;
    m_width = right - left;
}

bool EditorWorld::build(ResourceFile &resources)
{
    // build each map
    for (int i=0; i<m_maps.size(); ++i) {
        EditorMap * map = m_maps.at(i);
        bool ok = map->build(resources);

        if (! ok) {
            qDebug() << QObject::tr("Unable to build world: map ") << map->name() << QObject::tr(" failed to build.");
            return false;
        }
    }

    // compile world and put in resources
    QByteArray worldData;

    // character identifier
    worldData.append("W");

    // version
    worldData.append((const char *) &c_codeVersion, 4);

    // number of maps
    int mapCount = m_maps.size();
    worldData.append((const char *) &mapCount, 4);

    for (int i=0; i<m_maps.size(); ++i) {
        EditorMap * map = m_maps.at(i);

        int left = (int) round(map->left());
        int top = (int) round(map->top());
        int layer = 0; // TODO: support stories or something

        // map x,y,z
        worldData.append((const char *) &left, 4);
        worldData.append((const char *) &top, 4);
        worldData.append((const char *) &layer, 4);

        // map name
        int nameSize = map->name().size();
        worldData.append((const char *) &nameSize, 4);
        worldData.append(map->name());
    }

    resources.updateResource(m_name.toStdString(), worldData.constData(), worldData.size());

    return true;
}
