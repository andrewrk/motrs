#include "EditorWorld.h"

#include "EditorMap.h"
#include "EditorUtils.h"

#include <QDebug>
#include <QStringList>
#include <QDir>

EditorWorld::EditorWorld(QString file) :
    World()
{
    QVector< QPair<QString, QString> > props;

    m_good = EditorUtils::loadTextFile(file, props);

    if( ! m_good )
        return;

    for(int i=0; i<props.size(); ++i) {
        if( props[i].first.compare("version", Qt::CaseInsensitive) == 0 ) {
            int fileVersion = props[i].second.toInt();
            int codeVersion = 1;
            if( fileVersion != codeVersion ) {
                qDebug() << "Tried to open version  " << fileVersion <<
                    " with version " << codeVersion << " code.";
                this->m_good = false;
                return;
            }
        } else if( props[i].first.compare("map", Qt::CaseInsensitive) == 0 ) {
            // map=x,y,z,id
            QStringList coords = props[i].second.split(",");
            QDir dir(EditorUtils::dataDir());
            dir.cd("maps");
            QString mapFile = dir.absoluteFilePath(coords[3]);
            EditorMap * map = new EditorMap(mapFile);
            map->setPosition(coords[0].toDouble(), coords[1].toDouble(), coords[2].toInt());
            m_maps.push_back(map);
        } else {
            qDebug() << "Unrecognized World property: " << props[i].first;
            this->m_good = false;
            return;
        }
    }

    calculateBoundaries();
}

EditorWorld::~EditorWorld()
{

}
