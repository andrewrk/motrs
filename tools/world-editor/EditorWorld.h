#ifndef EDITORWORLD_H
#define EDITORWORLD_H

#include <QString>

#include "World.h"

class EditorWorld : public World
{
public:
    ~EditorWorld();

    static EditorWorld * load(QString file);

    void save();

private: //variables
    // name of the world, e.g. "tower1.world"
    QString m_name;

private: //methods
    EditorWorld();
};

#endif // EDITORWORLD_H
