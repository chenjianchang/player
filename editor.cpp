#include "editor.h"

#include <QIcon>

editor::editor()
{
    setWindowTitle(tr("editor"));
    setWindowIcon(QIcon(":/Icon/subtitle_editor.png"));
    this->hide();
}
