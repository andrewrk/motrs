#ifndef EDITORWORLD_H
#define EDITORWORLD_H

#include <QString>
#include <QList>

#include "ResourceFile.h"

class EditorMap;

class EditorWorld
{
public:
    ~EditorWorld();

    static EditorWorld * createEmpty(QString name);
    static EditorWorld * load(QString file);
    void save();

    void addMap(EditorMap * map);
    void calculateBoundaries();

    inline void setName(QString newName);
    inline QString name();

    inline const QList<EditorMap *> * maps();

    // cached boundary coordinates (calculated from contained maps)
    inline int left();
    inline int top();
    inline int width();
    inline int height();

    // compile the world and its dependencies, and put into resources file.
    // returns success
    bool build(ResourceFile & resources);

private: //variables
    static const int c_codeVersion;

    // name of the world, e.g. "tower1.world"
    QString m_name;

    QList<EditorMap *> m_maps;

    // holds cached values for the boundaries of the world
    double m_left;
    double m_top;
    double m_width;
    double m_height;

private: //methods
    EditorWorld();
};


inline void EditorWorld::setName(QString newName)
{
    m_name = newName;
}

inline QString EditorWorld::name()
{
    return m_name;
}

inline const QList<EditorMap *> * EditorWorld::maps()
{
    return &m_maps;
}

inline int EditorWorld::left()
{
    return m_left;
}
inline int EditorWorld::top()
{
    return m_top;
}
inline int EditorWorld::width()
{
    return m_width;
}
inline int EditorWorld::height()
{
    return m_height;
}

#endif // EDITORWORLD_H
