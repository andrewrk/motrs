#include "EditorResourceManager.h"

#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

QMap<QString, QPixmap *> EditorResourceManager::s_artPixmaps;
QMap<QString, QPixmap *> EditorResourceManager::s_graphicPixmaps;

QPixmap * EditorResourceManager::pixmapForGraphic(QString graphicName)
{
    if( s_graphicPixmaps.contains(graphicName) ) {
        return s_graphicPixmaps.value(graphicName);
    } else {
        // check if it exists as a static image
        QDir dir(dataDir());
        dir.cd("bitmaps");
        QString bitmapFile = dir.absoluteFilePath(graphicName);
        if( QFileInfo(bitmapFile).exists() ) {
            QPixmap * pixmap = new QPixmap(bitmapFile);
            s_graphicPixmaps.insert(graphicName, pixmap);
            return pixmap;
        }

        // check if it exists as an animation
        dir.cdUp();
        dir.cd("animations");
        QString animationDir = dir.absoluteFilePath(graphicName);
        if( QFileInfo(animationDir).exists() ) {
            // grab the first frame from the animation
            dir.cd(animationDir);
            QStringList frames = dir.entryList(QDir::NoDotAndDotDot|QDir::Files, QDir::Name);
            QString firstFrame = frames.first();
            QPixmap * pixmap = new QPixmap(dir.absoluteFilePath(firstFrame));
            s_graphicPixmaps.insert(graphicName, pixmap);
            return pixmap;
        }
        return NULL;
    }
}

QPixmap * EditorResourceManager::pixmapForArt(QString artName)
{
    if( s_artPixmaps.contains(artName) ){
        return s_artPixmaps.value(artName);
    } else {
        QDir dir(dataDir());
        dir.cd("art");
        QString artFile = dir.absoluteFilePath(artName);
        if( QFileInfo(artFile).exists() ) {
            QPixmap * pixmap = new QPixmap(artFile);
            s_artPixmaps.insert(artName, pixmap);
            return pixmap;
        }
        return NULL;
    }
}


bool EditorResourceManager::loadTextFile(QString filename, QVector< QPair<QString, QString> > & v)
{
    QFile infile(filename);

    if( ! infile.open(QIODevice::ReadOnly | QIODevice::Text) ){
        qDebug() << "Error opening " << filename;
        return false;
    }

    v.clear();

    QTextStream in(&infile);
    while(! in.atEnd()) {
        QString line = in.readLine().trimmed();
        // handle backslash at the end
        while( line.mid(line.size()-1,1).compare("\\") == 0 ) {
            if( in.atEnd() ) {
                qDebug() << "Syntax error reading " << filename
                    << ": Backslash at EOF and no next line";
                infile.close();
                return false;
            }
            line.remove(line.size()-1, 1);
            line.append(in.readLine().trimmed());
        }

        if( line.size() == 0 || line.at(0) == '#' ) {
            // comment. ignore
            continue;
        } else {
            // break into name-value pairs
            QStringList pairs = line.split("=");
            if( pairs.size() < 2 ) {
                infile.close();
                qDebug() << "Syntax error reading " << filename;
                return false;
            }
            QString name = pairs.takeFirst().trimmed();
            QString value = pairs.join("").trimmed();

            v.append(QPair<QString, QString>(name, value));
        }
    }

    infile.close();
    return true;
}



QString EditorResourceManager::dataDir()
{
    QSettings settings;
    return settings.value("paths/data", ".").toString();
}


QString EditorResourceManager::localDataDir()
{
    QDir dir("data");
    return dir.absolutePath();
}
