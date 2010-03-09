#ifndef EDITORUNIVERSE_H
#define EDITORUNIVERSE_H

#include <QString>
#include <QStringList>

class EditorUniverse
{
public:
    // load from disk
    static EditorUniverse * load(QString file);

    // save to disk
    void save();

    // changes a world's name, in memory and on disk.
    void renameWorld(QString oldName, QString newName);

    void removeWorld(QString worldName);

private: //variables
    QStringList m_worlds;

    // where this universe is saved
    QString m_name;

    // player entity
    QString m_player;

    int m_startWorld;
    int m_startX;
    int m_startY;
    int m_startZ;

private: //methods
    EditorUniverse();

};

#endif // EDITORUNIVERSE_H
