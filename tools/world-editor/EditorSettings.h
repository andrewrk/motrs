#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include "WorldView.h"

#include <QString>
#include <QByteArray>

// static class which manages saving and loading persistent settings
class EditorSettings
{
public:
    enum GridRenderType {
        None,
        Fast,
        Pretty,
        Solid,
    };

    static GridRenderType gridRenderType();
    static void setGridRenderType(GridRenderType value);

    static QString dataFolder();
    static void setDataFolder(QString value);

    static QByteArray worldEditorState();
    static void setWorldEditorState(QByteArray value);

    static QByteArray worldEditorGeometry();
    static void setWorldEditorGeometry(QByteArray value);

    static WorldView::MouseTool worldEditorToolLeft();
    static void setWorldEditorToolLeft(WorldView::MouseTool value);

    static WorldView::MouseTool worldEditorToolMiddle();
    static void setWorldEditorToolMiddle(WorldView::MouseTool value);

    static WorldView::MouseTool worldEditorToolRight();
    static void setWorldEditorToolRight(WorldView::MouseTool value);


};

#endif // EDITORSETTINGS_H
