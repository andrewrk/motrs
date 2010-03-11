#ifndef EDITORUNIVERSE_H
#define EDITORUNIVERSE_H

#include "ResourceFile.h"

#include <QString>
#include <QStringList>

class EditorWorld;

class EditorUniverse
{
public:
    // load from disk
    static EditorUniverse * load(QString file);

    // save to disk
    void save();

    // changes a world's name, in memory and on disk.
    void renameWorld(QString oldName, QString newName);

    void removeWorld(QString worldName);

    // go through the dependency chain, compile into binary, and pack into
    // resources. returns success
    bool build(ResourceFile & resources);

    // compile into binary. allocates memory. if size is supplied, the size
    // of the binary data is written to it.
    char * compile(int * size = NULL);

    // set the starting point
    void setStartWorld(EditorWorld * world);
    void setStartPosition(int x, int y, int story); // absolute coordinates

    inline int startX();
    inline int startY();
    inline int startLayer();
    QString startWorldName();

private: //variables
    static const int c_codeVersion;

    QStringList m_worlds;

    // where this universe is saved
    QString m_name;

    // player entity
    QString m_player;

    // index to m_worlds
    int m_startWorld;

    int m_startX;
    int m_startY;
    int m_startLayer; // layer

private: //methods
    EditorUniverse();

};

inline int EditorUniverse::startX()
{
    return m_startX;
}

inline int EditorUniverse::startY()
{
    return m_startY;
}

inline int EditorUniverse::startLayer()
{
    return m_startLayer;
}

#endif // EDITORUNIVERSE_H
