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

    inline int centerOffsetX();
    inline int centerOffsetY();

    inline int width();
    inline int height();

    QRectF geometry();

    inline QString name() const;

    inline EditorMap * parent();
    void setParent(EditorMap * map);

    void setCenter(int centerX, int centerY);
    inline int centerX();
    inline int centerY();

    inline int layer();
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

inline int EditorEntity::width()
{
    return m_graphic->width();
}

inline int EditorEntity::height()
{
    return m_graphic->height();
}

inline int EditorEntity::centerOffsetX()
{
    return m_centerOffsetX;
}

inline int EditorEntity::centerOffsetY()
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

inline int EditorEntity::centerX()
{
    return m_centerX;
}

inline int EditorEntity::centerY()
{
    return m_centerY;
}

inline int EditorEntity::layer()
{
    return m_layer;
}

inline void EditorEntity::setLayer(int layer)
{
    m_layer = layer;
}


#endif // EDITORENTITY_H
