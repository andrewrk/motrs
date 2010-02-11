#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

#include <QString>

// static class which manages saving and loading persistent settings
class EditorSettings
{
public:
    enum GridRenderType {
        None,
        Fast,
        Pretty,
    };

    static GridRenderType gridRenderType();
    static void setGridRenderType(GridRenderType value);

    static QString dataFolder();
    static void setDataFolder(QString value);
};

#endif // EDITORSETTINGS_H
