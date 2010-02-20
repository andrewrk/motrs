#ifndef EDITORENTITY_H
#define EDITORENTITY_H

#include "Entity.h"
#include "EditorGraphic.h"

class EditorEntity : public Entity
{
public:
    EditorEntity(QString file);

    inline bool isGood() { return m_good; }

    void draw(double screenX, double screenY);
private:
    bool m_good;
    EditorGraphic * m_graphic;
};

#endif // EDITORENTITY_H
