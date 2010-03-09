#include "EditorUniverse.h"

#include "EditorResourceManager.h"
#include "Debug.h"

#include <QPair>
#include <QDebug>
#include <QFile>
#include <QDir>
#include <QFileInfo>

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
            int codeVersion = 2;
            int fileVersion = prop.second.toInt();
            assert(codeVersion == fileVersion);
        } else if (prop.first.compare("world", Qt::CaseInsensitive) == 0) {
            out->m_worlds.append(prop.second);
        } else if (prop.first.compare("player", Qt::CaseInsensitive) == 0) {
            out->m_player = prop.second;
        } else if (prop.first.compare("start", Qt::CaseInsensitive) == 0) {
            QStringList values = prop.second.split(",");
            out->m_startWorld = values.at(0).toInt();
            out->m_startX = values.at(1).toInt();
            out->m_startY = values.at(2).toInt();
            out->m_startZ = values.at(3).toInt();
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

    out << "version=2\n\n";
    out << "# multiple world declarations allowed. Order is significant.\n";
    foreach (QString world, m_worlds)
        out << "world=" << world << "\n";

    out << "\n# player defines who you are.\n";
    out << "player=hero.entity\n";

    out << "\n# start is where you start in the universe.\n";
    out << "# start=worldIndex,x,y,z\n";
    out << "# x,y,z are coordinates of the center of the hero's feet.\n";
    out << "start=" << m_startWorld << "," << m_startX << "," << m_startY << "," << m_startZ << "\n";
}

void EditorUniverse::renameWorld(QString oldName, QString newName)
{
    for (int i=0; i<m_worlds.size(); ++i) {
        if (m_worlds.at(i) == oldName) {
            m_worlds.replace(i, newName);
            return;
        }
    }
    assert(false);
}
