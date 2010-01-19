#include "editorutils.h"

#include <QSettings>
#include <QFile>
#include <QTextStream>
#include <QStringList>
#include <QDebug>

bool EditorUtils::loadTextFile(QString filename, QVector< QPair<QString, QString> > & v)
{
    QFile infile(filename);

    if( ! infile.open(QIODevice::ReadOnly | QIODevice::Text) ){
        qDebug() << "Error opening " << filename;
        return false;
    }

    v.clear();

    QTextStream in(&infile);
    while(! in.atEnd()) {
        QString line = in.readLine().trimmed();

        if( line.size() == 0 || line.at(0) == '#' ) {
            // comment. ignore
            continue;
        } else {
            // break into name-value pairs
            QStringList pairs = line.split("=");
            if( pairs.size() < 2 ) {
                infile.close();
                qDebug() << "Syntax error reading " << filename;
                return false;
            }
            QString name = pairs.takeFirst();
            QString value = pairs.join("");

            v.append(QPair<QString, QString>(name, value));
        }
    }

    infile.close();
    return true;
}



QString EditorUtils::dataDir()
{
    QSettings settings;
    return settings.value("paths/data", ".").toString();
}
