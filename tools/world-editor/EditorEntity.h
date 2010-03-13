#ifndef EDITORENTITY_H
#define EDITORENTITY_H

#include "EditorGraphic.h"

class EditorMap;

// an instance of an Entity for placing on the map
class EditorEntity
{
public:
    static EditorEntity * load(QString file);

    inline EditorGraphic * graphic();

    inline int centerOffsetX() const;
    inline int centerOffsetY() const;

    inline int width() const;
    inline int height() const;

    // rectangle containing the entity, in map coordinates
    QRect geometry() const;

    inline QString name() const;

    inline EditorMap * parent() const;
    void setParent(EditorMap * map);

    void setCenter(int centerX, int centerY);
    inline int centerX() const;
    inline int centerY() const;

    inline int layer() const;
    inline void setLayer(int layer);
private: //variables
    EditorGraphic * m_graphic;
    QString m_name;
    EditorMap * m_parent;

    int m_centerOffsetX;
    int m_centerOffsetY;
    int m_centerX;
    int m_centerY;

    int m_layer;

    int m_radius;
    int m_speed;
    int m_mass;

private: //methods
    EditorEntity();
};

inline EditorGraphic * EditorEntity::graphic()
{
    return m_graphic;
}

inline int EditorEntity::width() const
{
    return m_graphic->width();
}

inline int EditorEntity::height() const
{
    return m_graphic->height();
}

inline int EditorEntity::centerOffsetX() const
{
    return m_centerOffsetX;
}

inline int EditorEntity::centerOffsetY() const
{
    return m_centerOffsetY;
}

inline QString EditorEntity::name() const
{
    return m_name;
}

inline EditorMap * EditorEntity::parent() const
{
    return m_parent;
}

inline int EditorEntity::centerX() const
{
    return m_centerX;
}

inline int EditorEntity::centerY() const
{
    return m_centerY;
}

inline int EditorEntity::layer() const
{
    return m_layer;
}

inline void EditorEntity::setLayer(int layer)
{
    m_layer = layer;
}


#endif // EDITORENTITY_H
