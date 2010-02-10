#ifndef EDITORENTITY_H
#define EDITORENTITY_H

#include "Entity.h"

#include <QPixmap>

class EditorEntity : public Entity
{
public:
    EditorEntity(QString file);

    inline bool isGood() { return m_good; }

    void draw(double screenX, double screenY);
private:
    bool m_good;
    QPixmap * m_pixmap;
};

#endif // EDITORENTITY_H
