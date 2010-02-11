#include "EditorSettings.h"

#include <QSettings>

EditorSettings::GridRenderType EditorSettings::gridRenderType()
{
    QSettings settings;
    return (GridRenderType)settings.value("editor/grid", Fast).toInt();

}
