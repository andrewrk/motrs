#ifndef EDITORENTITY_H
#define EDITORENTITY_H

#include "Entity.h"
#include "EditorGraphic.h"

class EditorMap;

class EditorEntity : public Entity
{
public:
    static EditorEntity * load(QString file);

    inline EditorGraphic * graphic();

    inline double centerOffsetX();
    inline double centerOffsetY();

    inline int width();
    inline int height();

    QRectF geometry();

    inline QString name() const;

    inline EditorMap * parent();
    void setParent(EditorMap * map);
private:
    EditorEntity();
    EditorGraphic * m_graphic;

    QString m_name;

    EditorMap * m_parent;
};

inline EditorGraphic * EditorEntity::graphic()
{
    return m_graphic;
}

inline int EditorEntity::width()
{
    return m_graphic->width();
}

inline int EditorEntity::height()
{
    return m_graphic->height();
}

inline double EditorEntity::centerOffsetX()
{
    return m_centerOffsetX;
}

inline double EditorEntity::centerOffsetY()
{
    return m_centerOffsetY;
}

inline QString EditorEntity::name() const
{
    return m_name;
}

inline EditorMap * EditorEntity::parent()
{
    return m_parent;
}

#endif // EDITORENTITY_H
