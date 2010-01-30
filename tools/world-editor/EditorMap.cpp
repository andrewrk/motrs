#include "EditorMap.h"

#include "EditorUtils.h"
#include "EditorTile.h"

#include <QPixmap>
#include <QDir>
#include <QDebug>
#include <QPainter>

QPainter * EditorMap::m_painter = NULL;

EditorMap::EditorMap(QString file)
{
    QVector< QPair<QString, QString> > props;

    m_good = EditorUtils::loadTextFile(file, props);

    if( ! m_good )
        return;

    int sizeX, sizeY, layerCount;
    int layerIndex = 0;

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
            layerCount = size.at(2).toInt();

            m_tiles = new Array3<int>(sizeX, sizeY, layerCount);
        } else if( props[i].first.compare("tile", Qt::CaseInsensitive) == 0 ) {
            // tile=shape,surface,graphicId
            QStringList tileProps = props[i].second.split(",");
            QString graphicFile = tileProps.at(2);
            QPixmap * pixmap = EditorUtils::pixmapForGraphic(graphicFile);
            EditorTile * tile = new EditorTile(
                (EditorTile::Shape)tileProps.at(0).toInt(),
                (EditorTile::SurfaceType)tileProps.at(1).toInt(),
                pixmap);
            m_palette.push_back(tile);
        } else if( props[i].first.compare("layer", Qt::CaseInsensitive) == 0 ) {
            // layer=0,0,...
            QStringList layer = props[i].second.split(",");
            int index = 0;
            for(int y = 0; y < sizeY; ++y) {
                for(int x = 0; x < sizeX; ++x) {
                    m_tiles->set(x,y,layerIndex,layer[index].toInt());
                    ++index;
                }
            }
            ++layerIndex;

        } else {
            qDebug() << "Unrecognized Map property: " << props[i].first;
            this->m_good = false;
            return;
        }
    }

    calculateBoundaries();
}

void EditorMap::draw(QPainter * p, double screenX, double screenY,
                     double screenWidth, double screenHeight, int layer)
{
    m_painter = p;
    Map::draw(screenX, screenY, screenWidth, screenHeight, layer);
}

EditorMap::~EditorMap()
{
}

void EditorMap::setLeft(double value)
{
    this->m_x = value;
}

void EditorMap::setTop(double value)
{
    this->m_y = value;
}

void EditorMap::setWidth(double value)
{
    int tileCount = (int)(value / Tile::size);
    if (tileCount > 0)
        m_tiles->redim(tileCount, m_tiles->sizeY(), m_tiles->sizeZ(), 0);
    calculateBoundaries();
}

void EditorMap::setHeight(double value)
{
    int tileCount = (int)(value / Tile::size);
    if (tileCount > 0)
        m_tiles->redim(m_tiles->sizeX(), tileCount, m_tiles->sizeZ(), 0);
    calculateBoundaries();
}
