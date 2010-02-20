#include "EditorGraphicTimer.h"

#include "EditorGraphic.h"

#include "moc_EditorGraphicTimer.cxx"

EditorGraphicTimer::EditorGraphicTimer(int msec)
{
    connect(&m_timer, SIGNAL(timeout()), this, SLOT(timeout()));
    m_timer.start(msec);
}

void EditorGraphicTimer::timeout()
{
    EditorGraphic::updateFrameCount();
}
