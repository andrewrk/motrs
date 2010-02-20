#ifndef EDITORWORLD_H
#define EDITORWORLD_H

#include <QString>

#include "World.h"

class EditorWorld : public World
{
public:
    ~EditorWorld();

    static EditorWorld * load(QString file);

private:
    EditorWorld();
};

#endif // EDITORWORLD_H
