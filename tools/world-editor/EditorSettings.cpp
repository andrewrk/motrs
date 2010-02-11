#include "EditorSettings.h"

#include <QSettings>

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
