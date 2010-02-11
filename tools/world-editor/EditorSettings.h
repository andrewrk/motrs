#ifndef EDITORSETTINGS_H
#define EDITORSETTINGS_H

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
};

#endif // EDITORSETTINGS_H
