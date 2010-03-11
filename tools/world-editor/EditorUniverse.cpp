#include "EditorUniverse.h"

#include "EditorWorld.h"
#include "EditorResourceManager.h"
#include "Debug.h"

#include <QPair>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>

const int EditorUniverse::c_codeVersion = 2;

EditorUniverse::EditorUniverse()
{
}

EditorUniverse * EditorUniverse::load(QString filename)
{
    QList< QPair<QString, QString> > props;
    bool ok = EditorResourceManager::loadTextFile(filename, props);

    assert(ok);
    if (! ok) {
        qDebug() << "Unable to load universe: Error loading " << filename;
        return NULL;
    }

    EditorUniverse * out = new EditorUniverse();

    out->m_name = QFileInfo(filename).fileName();

    QPair<QString, QString> prop;
    foreach (prop, props) {
        if (prop.first.compare("version", Qt::CaseInsensitive) == 0) {
            int fileVersion = prop.second.toInt();
            assert(c_codeVersion == fileVersion);
        } else if (prop.first.compare("world", Qt::CaseInsensitive) == 0) {
            out->m_worlds.append(prop.second);
        } else if (prop.first.compare("player", Qt::CaseInsensitive) == 0) {
            out->m_player = prop.second;
        } else if (prop.first.compare("start", Qt::CaseInsensitive) == 0) {
            QStringList values = prop.second.split(",");
            out->m_startWorld = values.at(0).toInt();
            out->m_startX = values.at(1).toInt();
            out->m_startY = values.at(2).toInt();
            out->m_startLayer = values.at(3).toInt();
        }
    }

    return out;
}

void EditorUniverse::save()
{
    QString filename = QDir(EditorResourceManager::universesDir()).absoluteFilePath(m_name);
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    assert(file.isOpen());
    if( ! file.isOpen() ){
        qDebug() << "Unable to open " << filename << " for writing";
        return;
    }

    QTextStream out(&file);

    out << "version=" << c_codeVersion << "\n\n";
    out << "# multiple world declarations allowed. Order is significant.\n";
    foreach (QString world, m_worlds)
        out << "world=" << world << "\n";

    out << "\n# player defines who you are.\n";
    out << "player=hero.entity\n";

    out << "\n# start is where you start in the universe.\n";
    out << "# start=worldIndex,x,y,z\n";
    out << "# x,y,z are coordinates of the center of the hero's feet.\n";
    out << "start=" << m_startWorld << "," << m_startX << "," << m_startY << "," << m_startLayer << "\n";
}

void EditorUniverse::renameWorld(QString oldName, QString newName)
{
    for (int i=0; i<m_worlds.size(); ++i) {
        if (m_worlds.at(i) == oldName)
            m_worlds.replace(i, newName);
    }
}

void EditorUniverse::removeWorld(QString worldName)
{
    m_worlds.removeOne(worldName);
}

char * EditorUniverse::compile(int * size)
{
    QByteArray out;

    // identifier character
    out.append("U");

    // code version
    out.append((const char *) &c_codeVersion, 4);

    // number of worlds
    int worldCount = m_worlds.size();
    out.append((const char *) &worldCount, 4);

    // names of worlds, in order
    foreach (QString worldName, m_worlds) {
        int worldNameSize = worldName.size();
        out.append((const char *) &worldNameSize, 4);
        out.append(worldName);
    }

    // player name
    int playerNameSize = m_player.size();
    out.append((const char *) &playerNameSize, 4);
    out.append(m_player);

    // start world index, x, y, z
    out.append((const char *) &m_startWorld, 4);
    out.append((const char *) &m_startX, 4);
    out.append((const char *) &m_startY, 4);
    out.append((const char *) &m_startLayer, 4);

    if (size != NULL)
        *size = out.size();

    return out.data();
}

bool EditorUniverse::build(ResourceFile & resources)
{
    // build each world
    foreach (QString worldName, m_worlds) {
        // load up the world and build it
        EditorWorld * world = EditorWorld::load(QDir(EditorResourceManager::worldsDir()).absoluteFilePath(worldName));
        assert(world);
        if (world == NULL) {
            qDebug() << QObject::tr("Unable to build universe: world ") << worldName << QObject::tr(" failed to load.");
            return false;
        }

        bool ok = world->build(resources);
        delete world;

        assert(ok);
        if (! ok) {
            qDebug() << QObject::tr("Unable to build universe: world ") << worldName << QObject::tr(" failed to build.");
            return false;
        }
    }

    // put the compiled universe into resources.dat
    int size;
    char * data = compile(&size);
    resources.updateResource(m_name.toStdString(), data, size);

    return true;
}


void EditorUniverse::setStartWorld(EditorWorld * world)
{
    QString name = world->name();
    m_startWorld = m_worlds.indexOf(name);

    if (m_startWorld == -1) {
        m_startWorld = m_worlds.size();
        m_worlds.append(name);
    }
}

void EditorUniverse::setStartPosition(int x, int y, int z)
{
    m_startX = x;
    m_startY = y;
    m_startLayer = z;
}

QString EditorUniverse::startWorldName()
{
    if (m_worlds.size() > 0)
        return m_worlds.at(m_startWorld);
    else
        return QString();
}
