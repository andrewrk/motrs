#ifndef EDITORUTILS_H
#define EDITORUTILS_H

#include <QVector>
#include <QString>
#include <QPair>
#include <QPixmap>

namespace EditorUtils
{
    bool loadTextFile(QString file, QVector< QPair<QString, QString> > & v);
    QString dataDir();

    // create a new QPixmap that represents the graphic in graphicName
    QPixmap * pixmapForGraphic(QString graphicName);
}

#endif // EDITORUTILS_H
