#include "EditorEntity.h"

#include "EditorResourceManager.h"
#include "WorldView.h"

#include <QDebug>
#include <QPainter>

EditorEntity::EditorEntity(QString file)
{
    QList< QPair<QString, QString> > props;

    m_good = EditorResourceManager::loadTextFile(file, props);

    if( ! m_good )
        return;

    for(int i=0; i<props.size(); ++i) {
        if( props[i].first.compare("version", Qt::CaseInsensitive) == 0 ) {
            int fileVersion = props[i].second.toInt();
            int codeVersion = 6;
            if( fileVersion != codeVersion ) {
                qDebug() << "Tried to open version  " << fileVersion <<
                    " with version " << codeVersion << " code.";
                this->m_good = false;
                return;
            }
        } else if( props[i].first.compare("contact", Qt::CaseInsensitive) == 0 ) {
            // contact=cx,cy,r
            QStringList contactProps = props[i].second.split(",");
            m_centerOffsetX = contactProps.at(0).toDouble();
            m_centerOffsetY = contactProps.at(1).toDouble();
            m_radius = contactProps.at(2).toDouble();
        } else if( props[i].first.compare("specs", Qt::CaseInsensitive) == 0 ) {
            // specs = speed, mass
            QStringList specs = props[i].second.split(",");
            m_speed = specs.at(0).toDouble();
            m_mass = specs.at(1).toDouble();
        } else if( props[i].first.compare("stand", Qt::CaseInsensitive) == 0 ) {
            // nw,w,sw,n,c,s,ne,e,se
            QStringList standGraphics = props[i].second.split(",");
            m_graphic = EditorResourceManager::graphic(standGraphics.at(5));
        } else if( props[i].first.compare("walk", Qt::CaseInsensitive) == 0 ) {
            // world editor doesn't care about this
        } else if( props[i].first.compare("run", Qt::CaseInsensitive) == 0 ) {
            // world editor doesn't care about this
        } else if( props[i].first.compare("sword", Qt::CaseInsensitive) == 0 ) {
            // world editor doesn't care about this
        } else {
            qDebug() << "Unrecognized Entity property: " << props[i].first
                << " = " << props[i].second;
            m_good = false;
            return;

        }
    }
}

void EditorEntity::draw(double screenX, double screenY)
{
    m_graphic->draw(*WorldView::painter(),
        (int)(m_centerX - m_centerOffsetX - screenX),
        (int)(m_centerY - m_centerOffsetY - m_altitude - screenY));
}
