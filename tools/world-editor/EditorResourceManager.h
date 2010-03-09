#ifndef EDITORRESOURCEMANAGER_H
#define EDITORRESOURCEMANAGER_H

#include "EditorGraphic.h"

#include <QList>
#include <QString>
#include <QPair>
#include <QMap>

class EditorResourceManager
{
public:
    // read a text file that uses the properties format into a list
    static bool loadTextFile(QString file, QList< QPair<QString, QString> > & out_list);

    // the folder that contains all the in game data
    static QString dataDir();

    // specific folders within dataDir
    static QString entitiesDir();
    static QString graphicsDir();
    static QString mapsDir();
    static QString objectsDir();
    static QString worldsDir();
    static QString universesDir();
    static QString universeFile();

    // the resources that don't have to do with in-game graphics (editor resources)
    static QString localDataDir();

    // get the EditorGraphic that represents the graphic name. don't worry
    // about calling it twice - it uses a cache.
    static EditorGraphic * graphic(QString name);

    static QString newUniqueMapName(QString worldName = QString());
private:
    static QMap<QString, EditorGraphic *> s_graphics;
};

#endif // EDITORRESOURCEMANAGER_H
