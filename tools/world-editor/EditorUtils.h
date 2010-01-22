#ifndef EDITORUTILS_H
#define EDITORUTILS_H

#include <QVector>
#include <QString>
#include <QPair>

namespace EditorUtils
{
    bool loadTextFile(QString file, QVector< QPair<QString, QString> > & v);
    QString dataDir();
}

#endif // EDITORUTILS_H
