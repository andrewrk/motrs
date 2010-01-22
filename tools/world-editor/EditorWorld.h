#ifndef EDITORWORLD_H
#define EDITORWORLD_H

#include <QString>

#include "World.h"

class EditorWorld : public World
{
public:
    EditorWorld(QString file);
    ~EditorWorld();

private:
};

#endif // EDITORWORLD_H
