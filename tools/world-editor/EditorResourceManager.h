#ifndef EDITORRESOURCEMANAGER_H
#define EDITORRESOURCEMANAGER_H

#include <QVector>
#include <QString>
#include <QPair>
#include <QPixmap>
#include <QMap>

class EditorResourceManager
{
public:
    static bool loadTextFile(QString file, QVector< QPair<QString, QString> > & v);
    static QString dataDir();
    static QString localDataDir();
    static QString objectsDir();

    // create a new QPixmap that represents the graphic in graphicName
    static QPixmap * pixmapForGraphic(QString graphicName);
    static QPixmap * pixmapForArt(QString artName);
private:
    static QMap<QString, QPixmap *> s_graphicPixmaps;
    static QMap<QString, QPixmap *> s_artPixmaps;
};

#endif // EDITORRESOURCEMANAGER_H
