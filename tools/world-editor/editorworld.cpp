#include "editorworld.h"

#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

EditorWorld::EditorWorld(QString file) :
    World()
{
    QFile infile(file);

    if( ! infile.open(QIODevice::ReadOnly | QIODevice::Text) ){
        qDebug() << "Error opening " << file;
        this->m_good = false;
        return;
    }

    m_maps.clear();

    QTextStream in(&file);
    while(! in.atEnd()) {
        QString line = in.readLine();

        if( line.trimmed().at(0) == '#' ) {
            // comment. ignore
            continue;
        } else {
            // break into name-value pairs
            QStringList pairs = line.split("=");
            if( pairs.size() < 2 ) {
                qDebug() << "Syntax error reading World: " << file;
                this->m_good = false;
                infile.close();
                return;
            }
            QString name = pairs.takeFirst();
            QString value = pairs.join("");

            if( name.compare("version", Qt::CaseInsensitive) == 0 ) {
                int fileVersion = value.toInt();
                int codeVersion = 1;
                if( fileVersion != codeVersion ) {
                    qDebug() << "Tried to open version  " << fileVersion <<
                        " with version " << codeVersion << " code.";
                    this->m_good = false;
                    infile.close();
                    return;
                }
            } else if( name.compare("map", Qt::CaseInsensitive) == 0 ) {
                // map=x,y,z,id
                QStringList coords = value.split(",");
                WorldMap wmap;
                wmap.x = coords.at(0).toDouble();
                wmap.y = coords.at(1).toDouble();
                wmap.z = coords.at(2).toInt();
                wmap.map = new EditorMap(coords.at(3));
            } else {
                qDebug() << "Unrecognized World property: " << name;
                this->m_good = false;
                infile.close();
                return;
            }
        }
    }

    m_good = true;
}

EditorWorld::~EditorWorld()
{

}
