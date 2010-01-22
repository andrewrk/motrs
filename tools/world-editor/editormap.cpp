#include "editormap.h"

#include "editorutils.h"
#include "editortile.h"

#include <QPixmap>
#include <QDir>
#include <QDebug>

EditorMap::EditorMap(QString file)
{
    QVector< QPair<QString, QString> > props;

    m_good = EditorUtils::loadTextFile(file, props);

    if( ! m_good )
        return;

    int sizeX, sizeY;

    // implicit null tile
    m_palette.clear();
    m_palette.push_back(Tile::nullTile());

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
        } else if( props[i].first.compare("size", Qt::CaseInsensitive) == 0 ) {
            // size in number of tiles
            // size=sizeX,sizeY
            QStringList size = props[i].second.split(",");
            sizeX = size.at(0).toInt();
            sizeY = size.at(1).toInt();

            // TODO: there is a discrepency between the file format, which specifies
            // a 2d array of tiles, and Map class, which has an Array3 for tiles
            m_tiles = new Array3<int>(sizeX, sizeY, 1);
        } else if( props[i].first.compare("tile", Qt::CaseInsensitive) == 0 ) {
            // tile=shape,surface,graphicId
            QStringList tileProps = props[i].second.split(",");
            QDir dir(EditorUtils::dataDir());
            dir.cd("bitmaps");
            QString pixmapFile = dir.absoluteFilePath(tileProps.at(2));
            QPixmap * pixmap = new QPixmap(pixmapFile);
            EditorTile * tile = new EditorTile(
                (EditorTile::Shape)tileProps.at(0).toInt(),
                (EditorTile::SurfaceType)tileProps.at(1).toInt(),
                pixmap);
            m_palette.push_back(tile);
        } else if( props[i].first.compare("layer", Qt::CaseInsensitive) == 0 ) {
            // layer=0,0,...
            QStringList layer = props[i].second.split(",");
            int index = 0;
            for(int z = 0; z < 1; ++z ) {
                for(int y = 0; y < sizeY; ++y) {
                    for(int x = 0; x < sizeX; ++x) {
                        m_tiles->set(x,y,z,layer[index].toInt());
                        ++index;
                    }
                }
            }
        } else {
            qDebug() << "Unrecognized Map property: " << props[i].first;
            this->m_good = false;
            return;
        }
    }
}

EditorMap::~EditorMap()
{
}
