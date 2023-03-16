#include "course.h"

Course::Course(QWidget* parent) : QWidget(parent)
{
    instancierWidgets();

    initialiserWidgets();

    positionnerWidgets();

    connecterSignauxSlots();

    initialiserFenetre();
}

Course::~Course()
{
}
