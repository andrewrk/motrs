#include "EditorUtils.h"

#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>
#include <QFileInfo>
#include <QDir>

bool EditorUtils::loadTextFile(QString filename, QVector< QPair<QString, QString> > & v)
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



QString EditorUtils::dataDir()
{
    QSettings settings;
    return settings.value("paths/data", ".").toString();
}

QPixmap * EditorUtils::pixmapForGraphic(QString graphicName)
{
    // check if it exists as a static image
    QDir dir(EditorUtils::dataDir());
    dir.cd("bitmaps");
    QString bitmapFile = dir.absoluteFilePath(graphicName);
    if( QFileInfo(bitmapFile).exists() )
        return new QPixmap(bitmapFile);

    // check if it exists as an animation
    dir.cdUp();
    dir.cd("animations");
    QString animationDir = dir.absoluteFilePath(graphicName);
    if( QFileInfo(animationDir).exists() ) {
        // grab the first frame from the animation
        dir.cd(animationDir);
        QStringList frames = dir.entryList(QDir::NoDotAndDotDot|QDir::Files, QDir::Name);
        QString firstFrame = frames.first();
        return new QPixmap(dir.absoluteFilePath(firstFrame));
    }

    return NULL;
}
