#include "EditorObject.h"

#include "EditorResourceManager.h"

#include <QFile>
#include <QDebug>
#include <QFileInfo>
#include <QBuffer>

EditorObject::EditorObject() :
    m_layerNames(),
    m_surfaceTypes(new Array3<Tile::SurfaceType>(1,1,1)),
    m_shapes(new Array3<Tile::Shape>(1,1,1)),
    m_name(QObject::tr("New Object")),
    m_description(""),
    m_graphics(),
    m_compiledGraphics(NULL)
{
    m_surfaceTypes->clear();
    m_shapes->clear();
    m_layerNames << QObject::tr("Layer 1");
    m_graphics.insert(0, new QList<ObjectGraphic *>());
}

EditorObject::~EditorObject()
{
}

EditorObject * EditorObject::load(QString file)
{
    QList< QPair<QString, QString> > props;

    if( ! EditorResourceManager::loadTextFile(file, props) )
        return NULL;

    EditorObject * out = new EditorObject();

    out->m_resourceName = QFileInfo(file).fileName();

    out->m_layerNames.clear();

    for(int i=0; i<props.size(); ++i) {
        const QPair<QString, QString> & pair = props.at(i);

        if( pair.first.compare("version", Qt::CaseInsensitive) == 0 ) {
            int codeVersion = 1;
            int fileVersion = pair.second.toInt();
            if( codeVersion != fileVersion ) {
                qDebug() << "EditorObject::load - file version does not match code version";
                return NULL; // TODO fix this memory leak
            }
        } else if( pair.first.compare("name", Qt::CaseInsensitive) == 0 ) {
            out->m_name = pair.second;
        } else if( pair.first.compare("size", Qt::CaseInsensitive) == 0 ) {
            QStringList sizes = pair.second.split(",");
            int sizeX = sizes.at(0).toInt();
            int sizeY = sizes.at(1).toInt();
            int sizeZ = sizes.at(2).toInt();
            out->m_surfaceTypes = new Array3<Tile::SurfaceType>(sizeX, sizeY, sizeZ);
            out->m_shapes = new Array3<Tile::Shape>(sizeX, sizeY, sizeZ);
        } else if( pair.first.compare("description", Qt::CaseInsensitive) == 0 ) {
            out->m_description = pair.second;
        } else if( pair.first.compare("surfaceTypes", Qt::CaseInsensitive) == 0 ) {
            QStringList surfaceTypes = pair.second.split(",");
            int i=0;
            for(int z=0; z<out->layerCount(); ++z) {
                for(int y=0; y<out->tileCountY(); ++y) {
                    for(int x=0; x<out->tileCountX(); ++x) {
                        out->m_surfaceTypes->set(x,y,z, (Tile::SurfaceType) surfaceTypes.at(i).toInt() );
                        ++i;
                    }
                }
            }
        } else if( pair.first.compare("shapes", Qt::CaseInsensitive) == 0 ) {
            QStringList shapes = pair.second.split(",");
            int i=0;
            for(int z=0; z<out->layerCount(); ++z) {
                for(int y=0; y<out->tileCountY(); ++y) {
                    for(int x=0; x<out->tileCountX(); ++x) {
                        out->m_shapes->set(x,y,z, (Tile::Shape) shapes.at(i).toInt() );
                        ++i;
                    }
                }
            }
        } else if( pair.first.compare("graphic", Qt::CaseInsensitive) == 0 ) {
            // create a graphic and add it to the list
            ObjectGraphic * graphic = new ObjectGraphic;
            // x,y,width,height,layer,graphicFile
            QStringList graphicProps = pair.second.split(",");
            graphic->x = graphicProps.at(0).toDouble();
            graphic->y = graphicProps.at(1).toDouble();
            graphic->width = graphicProps.at(2).toDouble();
            graphic->height = graphicProps.at(3).toDouble();
            graphic->layer = graphicProps.at(4).toInt();
            graphic->graphicName = graphicProps.at(5);
            graphic->graphic = EditorResourceManager::graphic(graphic->graphicName);
            out->graphics()->at(graphic->layer)->append(graphic);
        } else if( pair.first.compare("layerName", Qt::CaseInsensitive) == 0 ) {
            out->m_layerNames << pair.second;
        } else {
            qDebug() << "EditorObject::load - unrecognized property name: " << pair.first;
            return NULL; // TODO: fix this memory leak
        }
    }

    if( out->m_layerNames.size() != out->m_surfaceTypes->sizeZ() ) {
        // layers do not match
        qDebug() << "Layer names do not match actual layer size";
        return NULL; // TODO: fix this memory leak
    }

    return out;
}

void EditorObject::save(QString filename)
{
    QFile file(filename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);

    if( ! file.isOpen() ){
        qDebug() << "Unable to open " << filename << " for writing";
        return;
    }

    QTextStream out(&file);

    out << "version=1\n";
    out << "name=" << m_name << "\n";
    out << "size=" << tileCountX() << "," << tileCountY() << "," << layerCount() << "\n";
    out << "description=" << m_description << "\n";

    out << "\n# flatten a 3d grid in z,y,x order\n";
    QString surfaceTypes = "";
    QString shapes = "";
    for(int z=0; z<layerCount(); ++z) {
        for(int y=0; y<tileCountY(); ++y) {
            for(int x=0; x<tileCountX(); ++x) {
                surfaceTypes += QString::number(m_surfaceTypes->get(x,y,z)) + QString(",");
                shapes += QString::number(m_shapes->get(x,y,z)) + QString(",");
            }
            surfaceTypes += " \\\n";
            shapes += " \\\n";
        }
        surfaceTypes += "\\\n";
        shapes += "\\\n";
    }
    out << "surfaceTypes=" << surfaceTypes << "\n";
    out << "shapes=" << shapes << "\n";

    out << "\n# list of graphics and their locations\n";
    out << "# graphic=x,y,width,height,layer,graphicFile\n";
    for(int z=0; z<layerCount(); ++z) {
        QList<ObjectGraphic *> * list = m_graphics.at(z);
        for(int i=0; i<list->size(); ++i) {
            ObjectGraphic * graphic = list->at(i);
            out << "graphic=" << graphic->x << "," << graphic->y << ","
                << graphic->width << "," << graphic->height << "," << z
                << "," << graphic->graphicName << "\n";
        }
    }

    out << "\n# layer names\n";
    for(int z=0; z<layerCount(); ++z) {
        out << "layerName=" << m_layerNames.at(z) << "\n";
    }
}


Tile::Shape EditorObject::shape(int x, int y, int z)
{
    return m_shapes->get(x,y,z);
}

void EditorObject::setShape(int x, int y, int z, Tile::Shape shape)
{
    m_shapes->set(x,y,z,shape);
}

Tile::SurfaceType EditorObject::surfaceType(int x, int y, int z)
{
    return m_surfaceTypes->get(x,y,z);
}

void EditorObject::setSurfaceType(int x, int y, int z, Tile::SurfaceType surfaceType)
{
    m_surfaceTypes->set(x,y,z,surfaceType);
}

void EditorObject::addLayer(QString name)
{
    QString newLayerName = name.isEmpty() ? QObject::tr("Layer %1").arg(QString::number(layerCount()+1))
        : name;
    m_graphics.insert(m_shapes->sizeZ(), new QList<ObjectGraphic *>());
    m_shapes->redim(m_shapes->sizeX(), m_shapes->sizeY(), m_shapes->sizeZ()+1, (Tile::Shape) 0);
    m_surfaceTypes->redim(m_shapes->sizeX(), m_shapes->sizeY(), m_shapes->sizeZ(), (Tile::SurfaceType) 0);
    m_layerNames << newLayerName;
}

void EditorObject::deleteLayer(int index)
{
    m_shapes->deleteRowZ(index);
    m_surfaceTypes->deleteRowZ(index);
    m_layerNames.removeAt(index);
    m_graphics.removeAt(index); // TODO: fix this memory leak
}

void EditorObject::swapLayer(int i, int j)
{
    m_shapes->swapRowZ(i, j);
    m_surfaceTypes->swapRowZ(i, j);
    m_layerNames.swap(i, j);

    // swap graphics list pointers
    QList<ObjectGraphic *> * tmp = m_graphics.value(i);
    m_graphics.insert(i, m_graphics.value(j));
    m_graphics.insert(j, tmp);
}

void EditorObject::addTilesLeft(int amount)
{
    m_shapes->expandLeft(amount, (Tile::Shape) 0);
    m_surfaceTypes->expandLeft(amount, (Tile::SurfaceType) 0);
}

void EditorObject::addTilesRight(int amount)
{
    m_shapes->expandRight(amount, (Tile::Shape) 0);
    m_surfaceTypes->expandRight(amount, (Tile::SurfaceType) 0);
}

void EditorObject::addTilesTop(int amount)
{
    m_shapes->expandTop(amount, (Tile::Shape) 0);
    m_surfaceTypes->expandTop(amount, (Tile::SurfaceType) 0);
}

void EditorObject::addTilesBottom(int amount)
{
    m_shapes->expandBottom(amount, (Tile::Shape) 0);
    m_surfaceTypes->expandBottom(amount, (Tile::SurfaceType) 0);
}

int EditorObject::tileCountX()
{
    return m_shapes->sizeX();
}

int EditorObject::tileCountY()
{
    return m_shapes->sizeY();
}

int EditorObject::layerCount()
{
    return m_shapes->sizeZ();
}

void EditorObject::renameLayer(int index, QString newName)
{
    m_layerNames.replace(index, newName);
}

QString EditorObject::layerName(int index)
{
    return m_layerNames.at(index);
}

bool EditorObject::build(ResourceFile & resources)
{
    // create the container to hold the graphic ids
    delete m_compiledGraphics;
    m_compiledGraphics = new Array3<QString>(tileCountX(), tileCountY(), layerCount());

    Array3< QList<QImage> * > animations(tileCountX(), tileCountY(), layerCount());
    // initialize animation list
    for (int z=0; z<layerCount(); z++) {
        for (int y=0; y<tileCountY(); ++y) {
            for (int x=0; x<tileCountX(); ++x) {
                animations.set(x,y,z,new QList<QImage>());
            }
        }
    }

    // find out the greatest frames per second
    int fps = 0;
    for (int layerIndex=0; layerIndex<m_graphics.size(); ++layerIndex) {
        QList<ObjectGraphic *> * layer = m_graphics.at(layerIndex);
        for (int i=0; i<layer->size(); ++i) {
            ObjectGraphic * graphicInstance = layer->at(i);
            if (graphicInstance->graphic->framesPerSecond() > fps)
                fps = graphicInstance->graphic->framesPerSecond();
        }
    }

    // for each animation frame
    int frame = 0;
    while (true) {
        if (frame > 0) {
            // check if the animation has looped yet.
            bool firstFrame = true;
            for (int layerIndex=0; layerIndex<m_graphics.size(); ++layerIndex) {
                QList<ObjectGraphic *> * layer = m_graphics.at(layerIndex);
                for (int i=0; i<layer->size(); ++i) {
                    ObjectGraphic * graphicInstance = layer->at(i);
                    if (graphicInstance->graphic->frameCount() % frame != 0) {
                        firstFrame = false;
                        break;
                    }
                }
                if (! firstFrame)
                    break;
            }

            // this means all animations have looped and we're done
            // collecting frames
            if (firstFrame)
                break;
        }

        for (int z=0; z<layerCount(); ++z) {
            // render the layer once into a QImage
            QImage layerImage(tileCountX() * Tile::sizeInt, tileCountY() * Tile::sizeInt, QImage::Format_ARGB32);
            QPainter layerPainter(&layerImage);
            render(layerPainter, z, frame, fps);

            // for each tile, draw that tile into a different QImage
            for (int y=0; y<tileCountY(); ++y) {
                for (int x=0; x<tileCountX(); ++x) {
                    QImage tileImage(Tile::sizeInt, Tile::sizeInt, QImage::Format_ARGB32);
                    QPainter p(&tileImage);
                    p.setBackground(Qt::transparent);
                    p.eraseRect(0, 0, tileImage.width(), tileImage.height());
                    p.drawImage(0, 0, layerImage, x * Tile::sizeInt, y * Tile::sizeInt, Tile::sizeInt, Tile::sizeInt);

                    QList<QImage> * frames = animations.get(x,y,z);
                    frames->append(tileImage);
                }
            }
        }

        ++frame;
    }
    int frameCount = frame;

    // create a spritesheet for each tile
    for (int z=0; z<layerCount(); ++z) {
        for (int y=0; y<tileCountY(); ++y) {
            for (int x=0; x<tileCountX(); ++x ) {
                // create the spritesheet
                QList<QImage> * frames = animations.get(x,y,z);
                QImage spritesheet(Tile::sizeInt * frameCount, Tile::sizeInt, QImage::Format_ARGB32);
                QPainter p(&spritesheet);
                p.setBackground(Qt::transparent);
                p.eraseRect(0, 0, spritesheet.width(), spritesheet.height());
                for (int i=0; i<frameCount; ++i)
                    p.drawImage(i * Tile::sizeInt, 0, frames->at(i % frames->size()));

                // create the binary data
                QByteArray tile;
                // magic character
                tile.append("G");

                int codeVersion = 1;
                tile.append((char *) &codeVersion, 4);

                int graphicType = Graphic::gtAnimation;
                tile.append((char *) &graphicType, 4);

                int storageType = Graphic::stPNG;
                tile.append((char *) &storageType, 4);

                // color key: hardcode magenta
                char red = 255;
                char green = 0;
                char blue = 255;
                tile.append(&red, 1);
                tile.append(&green, 1);
                tile.append(&blue, 1);

                tile.append((char *) &frameCount, 4);
                tile.append((char *) &fps, 4);

                // frame width and height
                tile.append((char *) &Tile::sizeInt, 4);
                tile.append((char *) &Tile::sizeInt, 4);

                // save spritesheet into memory
                QByteArray imageData;
                QBuffer buffer(&imageData);
                buffer.open(QIODevice::WriteOnly);
                spritesheet.save(&buffer, "PNG");

                int imageDataSize = imageData.size();
                tile.append((char *) &imageDataSize, 4);

                tile.append(imageData);

                // come up with a name
                QString dash = "-";
                QString ext = ".ani";
                QString graphicName = m_name + dash + QString::number(x) +
                    dash + QString::number(y) + dash + QString::number(z) + ext;

                resources.updateResource(graphicName.toStdString(), tile.constData(), tile.size());
                m_compiledGraphics->set(x,y,z,graphicName);
            }
        }
    }

    // cleanup animation list
    for (int z=0; z<layerCount(); z++) {
        for (int y=0; y<tileCountY(); ++y) {
            for (int x=0; x<tileCountX(); ++x) {
                delete animations.get(x,y,z);
            }
        }
    }

    return true;
}

QString EditorObject::compiledGraphicAt(int x, int y, int z)
{
    assert(m_compiledGraphics != NULL);
    return m_compiledGraphics->get(x,y,z);
}

void EditorObject::render(QPainter &p, int layerIndex, int frame, int fps)
{
    // clear the screen
    p.setBackground(Qt::transparent);
    p.eraseRect(0, 0, tileCountX() * Tile::sizeInt, tileCountY() * Tile::sizeInt);

    QList<EditorObject::ObjectGraphic *> * layerGraphics = m_graphics.at(layerIndex);
    for(int i=0; i<layerGraphics->size(); ++i) {
        EditorObject::ObjectGraphic * graphicInstance = layerGraphics->at(i);
        EditorGraphic * graphic = graphicInstance->graphic;

        // calculate which frame to draw based on fps
        int drawFrame = (frame * graphic->framesPerSecond() / fps + graphic->offset()) % graphic->frameCount();
        p.drawPixmap(graphicInstance->x, graphicInstance->y, graphicInstance->width, graphicInstance->height, *graphic->toPixmap(drawFrame));
    }
}
