#ifndef EDITORWORLD_H
#define EDITORWORLD_H

#include <QString>

#include "World.h"

class EditorMap;

class EditorWorld : public World
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

private: //variables
    // name of the world, e.g. "tower1.world"
    QString m_name;

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


#endif // EDITORWORLD_H
