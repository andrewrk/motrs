#include "EditorSettings.h"

#include "EditorResourceManager.h"

#include <QSettings>
#include <QDir>
#include <QFile>

EditorSettings::GridRenderType EditorSettings::gridRenderType()
{
    QSettings settings;
    return (GridRenderType)settings.value("editor/grid", Fast).toInt();

}

void EditorSettings::setGridRenderType(GridRenderType value)
{
    QSettings settings;
    settings.setValue("editor/grid", (int)value);
}

QString EditorSettings::dataFolder()
{
    QSettings settings;
    return settings.value("paths/data", ".").toString();
}

void EditorSettings::setDataFolder(QString value)
{
    QSettings settings;
    settings.setValue("paths/data", value);
}

QByteArray EditorSettings::worldEditorState()
{
    QSettings settings;

    // load window/dock state
    QDir localDataDir(EditorResourceManager::localDataDir());
    QFile defaultLayoutFile(localDataDir.absoluteFilePath("default-layout"));
    QByteArray defaultLayout;
    bool haveDefaultLayout = false;
    if( defaultLayoutFile.open(QIODevice::ReadOnly) ) {
        defaultLayout = defaultLayoutFile.readAll();
        haveDefaultLayout = true;
    }

    return settings.value("windowState", haveDefaultLayout ? defaultLayout : QVariant()).toByteArray();
}

void EditorSettings::setWorldEditorState(QByteArray value)
{
    QSettings settings;
    settings.setValue("windowState", value);
}

QByteArray EditorSettings::worldEditorGeometry()
{
    QSettings settings;
    return settings.value("windowGeometry").toByteArray();
}

void EditorSettings::setWorldEditorGeometry(QByteArray value)
{
    QSettings settings;
    settings.setValue("windowGeometry", value);
}

WorldView::MouseTool EditorSettings::worldEditorToolLeft()
{
    QSettings settings;
    return (WorldView::MouseTool)settings.value("state/tool/left", WorldView::Arrow).toInt();
}

void EditorSettings::setWorldEditorToolLeft(WorldView::MouseTool value)
{
    QSettings settings;
    settings.setValue("state/tool/left", value);
}


WorldView::MouseTool EditorSettings::worldEditorToolMiddle()
{
    QSettings settings;
    return (WorldView::MouseTool)settings.value("state/tool/middle", WorldView::Pan).toInt();

}

void EditorSettings::setWorldEditorToolMiddle(WorldView::MouseTool value)
{
    QSettings settings;
    settings.setValue("state/tool/middle", value);
}


WorldView::MouseTool EditorSettings::worldEditorToolRight()
{
    QSettings settings;
    return (WorldView::MouseTool)settings.value("state/tool/right", WorldView::Eraser).toInt();
}

void EditorSettings::setWorldEditorToolRight(WorldView::MouseTool value)
{
    QSettings settings;
    settings.setValue("state/tool/right", value);
}

