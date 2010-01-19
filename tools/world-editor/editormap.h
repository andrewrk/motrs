#ifndef EDITORMAP_H
#define EDITORMAP_H

#include "Map.h"

#include <QString>

class EditorMap : public Map
{
public:
    // load a Map from a text format file
    EditorMap(QString file);
    ~EditorMap();
};

#endif // EDITORMAP_H
